#!/usr/bin/env python3
"""
@file remote_ops.py
@brief Perform remote operations on servers.
@author Kevin Briggs <kevin.briggs@ansys.com>
Copyright 2022, Ansys Inc.
@version 1
@date 2022-02-09
"""

import os
import sys
import shutil
import argparse
from pathlib import Path

import paramiko
from paramiko import SSHClient
from scp import SCPClient

import input_remote as inp



# *****************************************************************************
# ENVIRONMENT VARIABLES
# *****************************************************************************

USER = os.environ['VM_USER']
PASSWORD = os.environ['VM_PASSWORD']
PORT = os.environ.get('VM_PORT', 22) # default to port 22

# *****************************************************************************
# FUNCTIONS/CLASSES
# *****************************************************************************

# Setup command line.
def setup_parser():
    """
    @brief Setup and return the argument parser for this module.
    """
    parser = argparse.ArgumentParser(
        description='Perform remote operations (scp, commands) with servers.'
    )

    subparsers = parser.add_subparsers(title='subcommands',
        help='valid commands')

    # add parser for scatter operations
    parser_scatter = subparsers.add_parser('scatter', aliases=['s'],
        help='copy files from local machine to all servers')
    parser_scatter.add_argument('paths', metavar='PATHS', type=Path, nargs='+',
        help='file paths, where the last path is the destination and all other'
        ' paths are the sources. NOTE: for best results, make remote paths absolute.')
    parser_scatter.set_defaults(func=scatter)

    # add parser for gather operations
    parser_gather = subparsers.add_parser('gather', aliases=['g'],
        help='copy files from all servers to local')
    parser_gather.add_argument('paths', metavar='PATHS', type=Path, nargs='+',
        help='file paths, where the last path specifies the destination '
        'directory to be found under a server-specific directory, and all other '
        'paths are the sources.  NOTE: for best results, make remote paths absolute.')
    parser_gather.set_defaults(func=gather)

    # add parser for remote commands over ssh
    parser_cmd = subparsers.add_parser('run', aliases=['r'],
        help='perform commands on servers')
    parser_cmd.add_argument('commands', metavar='COMMANDS', type=str, nargs='+',
            help='commands to execute. NOTE: commands will be executed '
            'synchronously on each server. For long-running commands, consider '
            'using the `nohup` utility on linux in case ssh client is '
            'disconnected.')
    parser_cmd.set_defaults(func=commands)

    return parser


# SCP HELPER FUNCTIONS ********************************************************

def progress(filename, size, sent):
    sys.stdout.write("%s's progress: %.2f%%   \r" %
            (filename, float(sent)/float(size)*100) )

def createSSHClient(server, port, user, password):
    client = paramiko.SSHClient()
    client.load_system_host_keys()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(server, port, user, password)
    return client


# SCATTER OPERATIONS **********************************************************

def copy_to_all(file_path, target_path):
    for server in inp.SERVER_DICT.values():
        print("Copying ", str(file_path), "to", str(target_path), "on",
                str(server))
        ssh = createSSHClient(server, PORT, USER, PASSWORD)
        scp = SCPClient(ssh.get_transport(), progress=progress)
        scp.put(file_path, recursive=True, remote_path=target_path.as_posix())
        ssh.close()

def scatter(args):
    # ensure there are at least two paths provided
    if len(args.paths) < 2:
        raise Exception("There must be at least two paths provided.")

    print("sources:", *args.paths[:-1], sep="\n")
    print("\n")
    print("destination:", args.paths[-1], sep="\n")
    print("\n")

    for source_path in args.paths[:-1]:
        copy_to_all(source_path, args.paths[-1])


# GATHER OPERATIONS ***********************************************************

def copy_from_all(file_path, case_subdir):
    for key, server in inp.SERVER_DICT.items():
        print("Copying ", str(file_path), "from", str(server))

        # move to respective server directory
        dest_dir = Path(key, case_subdir)
        src_filename = Path(file_path).name
        dst_path = dest_dir / src_filename
        dest_dir.mkdir(exist_ok=True, parents=True)

        ssh = createSSHClient(server, PORT, USER, PASSWORD)
        scp = SCPClient(ssh.get_transport(), progress=progress)
        scp.get(remote_path=file_path.as_posix(), local_path=dst_path,
                recursive=True)
        ssh.close()

def gather(args):
    # ensure there are at least two paths provided
    if len(args.paths) < 2:
        raise Exception("There must be at least two paths provided.")

    print("\n")
    print("sources:", *args.paths[:-1], sep="\n")
    print("destination:", args.paths[-1], sep="\n")

    for source_path in args.paths[:-1]:
        copy_from_all(source_path, args.paths[-1])



# COMMAND OPERATIONS **********************************************************

def run_command(cmd):

    for key, server in inp.SERVER_DICT.items():
        print("Executing ", cmd, "on", str(server))
        with paramiko.SSHClient() as ssh:
            ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
            ssh.connect(hostname=server, port=PORT, username=USER, password=PASSWORD)
            ssh_stdin, ssh_stdout, ssh_stderr = ssh.exec_command(cmd)

            print('stdout:')
            print(ssh_stdout.read().decode('UTF-8'))
            print('stderr:')
            print(ssh_stderr.read().decode('UTF-8'))
            ssh.close()

def commands(args):
    print("commands:", *args.commands, sep="\n")

    for cmd in args.commands:
        run_command(cmd)


# *****************************************************************************
# MAIN
# *****************************************************************************

def main():
    args = setup_parser().parse_args()
    args.func(args)

if __name__ == '__main__':
    main()
