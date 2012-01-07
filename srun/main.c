//Author: Ankudinov Alexander

#include <saferun.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <glib.h>

#include "config.h"
#include "utils.h"

struct saferun_jail jail;
struct saferun_limits limits;
struct saferun_task task;
struct saferun_stat stat;

gchar *user;
gchar *group;
gchar *in_file;
gchar *out_file;
gchar *err_file;
gboolean show_version = FALSE;

static GOptionEntry entries[] =
{
    { "mem",      'm', 0, G_OPTION_ARG_INT64,  &limits.mem,    "Memory limit in bytes", "N" },
    { "time",     't', 0, G_OPTION_ARG_INT,    &limits.time,   "User+System time limit in milliseconds", "N" },
    { "rtime",    'r', 0, G_OPTION_ARG_INT,    &limits.rtime,  "Real time limit in milliseconds", "N" },
    
    { "hostname",  0 , 0, G_OPTION_ARG_STRING, &jail.hostname, "Change computer hostname", "name" },
    { "chroot",   'c', 0, G_OPTION_ARG_STRING, &jail.chroot,   "Do a chroot", "dir" },
    { "chdir",    'd', 0, G_OPTION_ARG_STRING, &jail.chdir,    "Change working directory (after chroot)", "dir" },
    { "user",     'u', 0, G_OPTION_ARG_STRING, &user,          "Run program as this user", "name" },
    { "group",    'g', 0, G_OPTION_ARG_STRING, &group,         "Run program as this group", "name" },
    
    { "in",  'i', 0, G_OPTION_ARG_FILENAME, &in_file,  "Redirect program stdin to file", "file" },
    { "out", 'o', 0, G_OPTION_ARG_FILENAME, &out_file, "Redirect program stdout to file", "file" },
    { "err", 'e', 0, G_OPTION_ARG_FILENAME, &err_file, "Redirect program stderr to file", "file" },

    
    { "version",  'v', 0, G_OPTION_ARG_NONE,   &show_version,  "Show version and exit", NULL },
    { NULL }
};

void set_default_options()
{
    user = "nobody";
    group = "nogroup";
    in_file = out_file = err_file = NULL;
    
    limits.mem = 64*1024*1024;
    limits.time = 1000;
    limits.rtime = 2 * limits.time;
    
    jail.chroot = NULL;
    jail.chdir = NULL;
    jail.hostname = NULL;
    
    task.jail = &jail;
    task.limits = &limits;
    task.stdin_fd = 0;
    task.stdout_fd = 1;
    task.stderr_fd = 2;
}

void parse_options(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;

    // parse options with glib
    context = g_option_context_new("[--] file ... - safely run program with limits");
    g_option_context_add_main_entries(context, entries, NULL);
    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        printf("option parsing failed: %s\n", error->message);
        printf("see --help for more information\n");
        exit(1);
    }
    
    jail.uid = uid_by_name(user);
    jail.gid = gid_by_name(group);

    if (in_file)
        task.stdin_fd = openfd(in_file, "r"); 
    if (out_file)
        task.stdout_fd = openfd(out_file, "w"); 
    if (err_file)
        task.stderr_fd = openfd(err_file, "w");

    task.argv = &argv[1];
}

char * result_str[] = {"OK", "RE", "TL", "ML", "SV"};

int main(int argc, char *argv[])
{
    set_default_options();
    parse_options(argc, argv);

    if (show_version) {
        g_printf("version: %s\n", SRUN_VERSION);
        return 1;
    }

    if (argc < 2) {
        g_print ("Error: Nothing to run\n");
        return 1;
    }
    
    char cgname[21];
    snprintf(cgname, 20, "srun%d", getpid());
    saferun_inst * inst = saferun_init(cgname);
    
    int res = saferun_run(inst, &task, &stat);
    
    if (res) {
        printf("Error: library error\n");
    } else {
        printf("\nresult = %s\nmem = %lld\ntime = %ld\nrtime = %ld\nstatus = %d\n",
               result_str[stat.result], stat.mem, stat.time, stat.rtime, stat.status);
        print_exit_status(stat.status);
    }
    
    saferun_fini(inst);

    if (!res && !stat.result)
        return 0;
    else
        return 1;
}
