/* check_by_ssh_master - Execute commands with SSH master connections
 *
 * Copyright (C) 2011 Oskar Liljeblad
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Run check_snmp_if --help for usage information.
 * Written by Oskar Liljeblad <oskar@osk.mine.nu>.
 *
 */

#include <config.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include "nagios.h"

static int received_signal = 0;
static int child_pid = 0;

static void
__attribute__ ((format (printf, 1, 2), noreturn))
nagios_die (const char *format, ...) 
{
    va_list arg;

    va_start(arg, format);
    vprintf(format, arg);
    va_end(arg);

    exit(NAGIOS_UNKNOWN);
}

static void
__attribute__ ((format (printf, 1, 2)))
nagios_warn (const char *format, ...) 
{
    va_list arg;

    va_start(arg, format);
    vprintf(format, arg);
    va_end(arg);
}

static int
escape_arguments(char **argv, int argc, char **out)
{
    size_t size = 0;
    char *p;

    for (size_t i = 0; i < argc; i++) {
        if (size != 0)
            size++;
        for (char *arg = argv[i]; *arg; arg++) {
            switch (*arg) {
            case '\n':
                size += 5;
                break;
            case 'A' ... 'Z':
            case 'a' ... 'z':
            case '0' ... '9': 
            case '_':
                size++;
                break;
            default:
                size += 2;
                break;
            }
        }
    }

    if ((p = malloc(size+1)) == NULL) {
        nagios_warn("cannot allocate %zu bytes\n", size);
        return NAGIOS_UNKNOWN;
    }

    *out = p;
    for (size_t i = 0; i < argc; i++) {
        if (p != *out)
            *p++ = ' ';
        for (char *arg = argv[i]; *arg; arg++) {
            switch (*arg) {
            case '\n':
                *p++ = '$';
                *p++ = '\'';
                *p++ = '\\';
                *p++ = 'n';
                *p++ = '\'';
                break;
            case 'A' ... 'Z':
            case 'a' ... 'z':
            case '0' ... '9': 
            case '_':
                *p++ = *arg;
                break;
            default:
                *p++ = '\\';
                *p++ = *arg;
                break;
            }
        }
    }
    *p = '\0';

    return NAGIOS_OK;
}

static int
run_ssh_command_v(bool quiet, char **ssh_argv)
{
    int child_status;

    child_pid = fork();
    if (child_pid < 0) {
        nagios_warn("cannot create process: %m\n");
        return NAGIOS_UNKNOWN;
    }
    if (child_pid == 0) {
        if (quiet) {
            int fd = open("/dev/null", O_WRONLY);
            if (fd >= 0) {
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
            } else {
                close(STDOUT_FILENO);
                close(STDERR_FILENO);
            }
        }
        execvp(ssh_argv[0], ssh_argv);
        nagios_die("cannot execute %s: %m\n", ssh_argv[0]);
    }
    if (waitpid(child_pid, &child_status, 0) < 0) {
        nagios_warn("cannot get process completion status: %m\n");
        return NAGIOS_UNKNOWN;
    }
    if (WIFSIGNALED(child_status)) {
        nagios_warn("%s terminated by signal %d\n", ssh_argv[0], WTERMSIG(child_status));
        return NAGIOS_UNKNOWN;
    }
    if (!WIFEXITED(child_status)) {
        nagios_warn("%s ended unexpectedly\n", ssh_argv[0]);
        return NAGIOS_UNKNOWN;
    }

    return WEXITSTATUS(child_status);
}

static int
run_ssh_command(bool quiet, char *command, ...)
{
    va_list args;
    int ssh_argc = 1;

    va_start(args, command);
    for (ssh_argc = 1; va_arg(args, char *) != NULL; ssh_argc++);
    va_end(args);

    char *ssh_argv[ssh_argc + 1];
    ssh_argv[0] = command;
    ssh_argc = 1;

    va_start(args, command);
    while ((ssh_argv[ssh_argc++] = va_arg(args, char *)) != NULL);
    va_end(args);
    ssh_argv[ssh_argc] = NULL;

    return run_ssh_command_v(quiet, ssh_argv);
}

static void
signal_received (int signum)
{
    received_signal = signum;
    if (child_pid != 0)
        kill(child_pid, SIGTERM); /* Ignore errors */
}

static void
start_master (const char *socket_file, const char *target)
{
    struct stat file_stat;

    if (stat(socket_file, &file_stat) < 0) {
        if (errno != ENOENT)
            nagios_die("cannot get file status for `%s': %m\n", socket_file);
        int child_rc = run_ssh_command(false, "ssh", "-oBatchMode=yes", "-M", "-S", socket_file, "-n", "-f", "-N", target, NULL);
        if (child_rc != 0)
            nagios_die("%s process ended with status %d\n", "ssh", child_rc);
    } else {
        int child_rc = run_ssh_command(true, "ssh", "-q", "-O", "check", "-S", socket_file, target, NULL);
        if (child_rc != 0) {
            if (unlink(socket_file) < 0)
                nagios_die("cannot unlink `%s': %m\n", socket_file);
            child_rc = run_ssh_command(false, "ssh", "-oBatchMode=yes", "-M", "-S", socket_file, "-n", "-f", "-N", target, NULL);
            if (child_rc != 0)
                nagios_die("%s process ended with status %d\n", "ssh", child_rc);
        }
    }
}

static void
stop_master (const char *socket_file, const char *target)
{
    run_ssh_command(true, "ssh", "-q", "-O", "exit", "-S", *socket_file, target, NULL); /* Ignore errors */
    /* XXX remove socket file? */
}

static void
update_sessions_file (const char *target, char **sessions_file, char **socket_file, bool increase, unsigned int max_sessions, const char *state_dir)
{
    int sessions_fd;

    if (max_sessions == 0) {
        if (increase) {
            if (asprintf(socket_file, "%s/%s.master", state_dir, target) < 0)
                nagios_die("cannot format string: %m\n");
            start_master(*socket_file, target);
        }
        return;
    }

    if (increase) {
        if (asprintf(sessions_file, "%s/%s.sessions", state_dir, target) < 0)
            nagios_die("cannot format string: %m\n");

        sessions_fd = open(*sessions_file, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
        if (sessions_fd < 0) {
            if (errno != EEXIST)
                nagios_die("cannot create `%s': %m\n", *sessions_file);
            sessions_fd = open(*sessions_file, O_RDWR);
        }
    } else {
        sessions_fd = open(*sessions_file, O_RDWR);
    }
    if (sessions_fd < 0)
        nagios_die("cannot open `%s': %m\n", *sessions_file);

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    if (fcntl(sessions_fd, F_SETLKW, &lock) < 0)
        nagios_die("cannot lock `%s': %m\n", *sessions_file);

    FILE *sessions_fh = fdopen(sessions_fd, "r+");
    if (sessions_fh == NULL)
        nagios_die("cannot open `%s': %m\n", *sessions_file);

    unsigned int session_count = 0;
    if (fscanf(sessions_fh, "%u", &session_count) < 0) {
        if (ferror(sessions_fh))
            nagios_die("cannot read `%s': %m\n", *sessions_file);
    }
    /* If an integer could not be read, assume session count is zero */

    if (increase) {
        if (session_count == UINT_MAX)
            nagios_die("max number of sessions reached (%u)\n", UINT_MAX);
        if (asprintf(socket_file, "%s/%s.master-%u", state_dir, target, session_count/max_sessions) < 0)
            nagios_die("cannot format string: %m\n");
        start_master(*socket_file, target);
        session_count++;
    } else {
        if (session_count == 0)
            nagios_die("session count already zero\n");
        session_count--;
        if (session_count % max_sessions == 0 && session_count/max_sessions > 0)
            stop_master(*socket_file, target);
    }

    if (fseek(sessions_fh, 0, SEEK_SET) < 0)
        nagios_die("cannot seek in `%s': %m\n", *sessions_file);
    if (fprintf(sessions_fh, "%u", session_count) < 0 || fflush(sessions_fh) != 0)
        nagios_die("cannot write to `%s': %m\n", *sessions_file);

    long pos = ftell(sessions_fh);
    if (pos < 0)
        nagios_die("cannot get position in `%s': %m\n", *sessions_file);
    if (ftruncate(sessions_fd, pos) < 0)
        nagios_die("cannot truncate `%s': %m\n", *sessions_file);
    if (fclose(sessions_fh) != 0)
        nagios_die("cannot close `%s': %m\n", *sessions_file);
}

int
main (int argc, char **argv)
{
    enum {
        OPT_HELP = CHAR_MAX+1,
        OPT_VERSION,
    };
    struct option long_opts[] = {
        { "target", required_argument, NULL, 'T' },
        { "timeout", required_argument, NULL, 't' },
        { "escape", no_argument, NULL, 'e' },
        { "state-directory", required_argument, NULL, 'S' },
        { "max-sessions", required_argument, NULL, 'm' },
        { "help", no_argument, NULL, OPT_HELP },
        { "version", no_argument, NULL, OPT_VERSION },
        { 0, },
    };
    int c;
    char *target = NULL;
    char *state_dir = NULL;
    char *socket_file;
    char *sessions_file;
    struct stat file_stat;
    bool escape = false;
    unsigned int max_sessions = 10;
    unsigned int timeout = 30;

    while ((c = getopt_long(argc, argv, "+et:T:S:m:", long_opts, NULL)) != -1) {
        switch (c) {
        case 'T':
            target = optarg;
            break;
        case 't':
            if (sscanf(optarg, "%u", &timeout) != 1) /* Ignore other errors */
                nagios_die("invalid value for --timeout/-t\n");
            break;
        case 'm':
            if (sscanf(optarg, "%u", &max_sessions) != 1) /* Ignore other errors */
                nagios_die("invalid value for --max-sessions/-m\n");
            break;
        case 'S':
            state_dir = optarg;
            break;
        case 'e':
            escape = true;
            break;
        case OPT_HELP:
            printf("Usage: %s -T [USER@]HOST [OPTION]... COMMAND...\n", argv[0]);
            puts("Execute a command on a remote host with SSH master connections.\n");
            puts("  -T, --target=[USER@]HOST          host to connect to (optional username)");
            puts("  -t, --timeout=SECONDS             max execution time (0 for no limit)");
            puts("  -e, --escape                      escape meta-characters");
            puts("  -m, --max-sessions=COUNT          max sessions per master (default 10)");
            puts("  -S, --state-directory=DIRECTORY   directory for master socket file");
            puts("\nIf --target is specified as the empty string, `-' or `$', the command will be");
            puts("run directly on the local host. Specify `0' for --max-sessions for an unlimited");
            puts("number of sessions.");
            printf("\nReport bugs to <%s>\n", PACKAGE_BUGREPORT);
            exit(NAGIOS_OK);
        case OPT_VERSION:
	    printf("%s %s\n", PACKAGE_NAME, PACKAGE_VERSION);
	    puts("Copyright (C) 2011 Oskar Liljeblad");
	    puts("License AGPLv3+: GNU Affero GPL version 3 or later <http://gnu.org/licenses/agpl.html>.");
	    puts("This is free software: you are free to change and redistribute it.");
	    puts("There is NO WARRANTY, to the extent permitted by law.");
	    puts("\nWritten by Oskar Liljeblad.");
            exit(NAGIOS_OK);
        case '?':
            exit(NAGIOS_UNKNOWN);
        }
    }


    if (target == NULL)
        nagios_die("missing target parameter\n");
    if (optind == argc)
        nagios_die("missing command operand\n");

    if (*target == '\0' || strcmp(target, "-") == 0 || strcmp(target, "$") == 0) {
        execvp(argv[optind], argv+optind);
        int exec_errno = errno;
        nagios_warn("cannot execute `%s': %m\n", argv[optind]);
        /* Try to emulate exit codes of the shell (bash) */
        exit(exec_errno == ENOENT ? 127 : 126);
    }

    /* Assume local commands handle timeouts themselves. */
    if (timeout != 0) {
        if (signal(SIGALRM, signal_received) < 0
                || signal(SIGTERM, signal_received) < 0
                || signal(SIGINT, signal_received) < 0
                || signal(SIGUSR1, signal_received) < 0
                || signal(SIGHUP, signal_received) < 0)
            nagios_die("cannot set up signal handler: %m\n");
        alarm(timeout);
    }

    if (state_dir == NULL) {
        if (asprintf(&state_dir, "%s/%s-%d", P_tmpdir, PACKAGE_TARNAME, geteuid()) < 0)
            nagios_die("cannot format string: %m\n");
    }
    if (stat(state_dir, &file_stat) < 0) {
        if (errno != ENOENT)
            nagios_die("cannot get file status for `%s': %m\n", state_dir);
        if (mkdir(state_dir, 0700) < 0)
            nagios_die("cannot create directory `%s': %m\n", state_dir);
    } else {
        if (!S_ISDIR(file_stat.st_mode))
            nagios_die("`%s' is not a directory\n", state_dir);
    }

    update_sessions_file(target, &sessions_file, &socket_file, true, max_sessions, state_dir);

    int status = NAGIOS_OK;
    if (received_signal == 0) {
        if (escape) {
            char *cmd;
            status = escape_arguments(argv+optind, argc-optind, &cmd);
            if (status == NAGIOS_OK)
                status = run_ssh_command(false, "ssh", "-S", socket_file, target, "--", cmd, NULL);
        } else {
            char *ssh_argv[5 + argc - optind + 1];
            ssh_argv[0] = "ssh";
            ssh_argv[1] = "-S";
            ssh_argv[2] = socket_file;
            ssh_argv[3] = target;
            ssh_argv[4] = "--";
            memcpy(ssh_argv + 5, argv + optind, (argc - optind + 1) * sizeof(char *));
            status = run_ssh_command_v(false, ssh_argv);
        }
    }

    update_sessions_file(target, &sessions_file, &socket_file, false, max_sessions, state_dir);

    if (received_signal == SIGALRM)
        nagios_die("command timeout\n");
    if (received_signal != 0)
        nagios_die("terminated by signal %d\n", received_signal);

    exit(status);
}
