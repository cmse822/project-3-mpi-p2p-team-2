# Title: Generate job files for the blocking Ring Shift test at different configurations.
#
#
# Author: Team 2 of CMSE 822 class.
# Date: 03/07/2024
# ===========================================================================================

# Importing the required libraries.
import os

# User defined variables.
Config = [
    {'NumNodes': 1, 'Ntasks': [2, 4, 8, 16, 32, 64, 128]}, 
    {'NumNodes': 2, 'Ntasks': [2, 4, 8, 16, 32, 64, 128]}, 
    {'NumNodes': 4, 'Ntasks': [4, 8, 16, 32, 64, 128]}, 
    {'NumNodes': 8, 'Ntasks': [8, 16, 32, 64, 128]}, 
    {'NumNodes': 16, 'Ntasks': [16, 32, 64, 128]}
]


def main():

    for conf in Config:
        Nnode = conf['NumNodes']
        for Ntasks in conf['Ntasks']:
            Ntask_node = int(Ntasks / Nnode)
            Name = f'n{Nnode}_t{Ntasks}'
            # Define the header.
            cont  = f"#!/bin/bash \n"
            cont += f"########## Define Resources Needed with SBATCH Lines ##########\n\n"
            cont += f"#SBATCH --time=00:30:00 \n"
            cont += f"#SBATCH --ntasks={Ntasks} \n"
            cont += f"#SBATCh --nodes={Nnode} \n"
            cont += f"#SBATCH --ntasks-per-node={Ntask_node} \n" 
            cont += f"#SBATCH --mem-per-cpu=100MB \n"
            cont += f"#SBATCH --job-name {Name} \n"
            cont += f"#SBATCH --nodelist=amr-[185-{185+Nnode-1}] \n\n\n"
            # Define the compilation part.
            cont += f"# Compiling part \n"
            cont += f"# mpicxx src/RingBlocking.cpp -o RingBlocking \n\n\n"
            # Define the bash part.
            cont += f"# Bash part. \n"
            cont += f"MessageSize=(2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216) \n"
            cont += 'for Size in "${MessageSize[@]}"; do \n'
            cont += f'\tmpiexec -n "{Ntasks}" RingBlocking  "$Size" "100" data/Ring_{Name}.csv \n'
            cont += f'done \n'

            # Save the job file.
            JobName = f'submitRing_{Name}.sb'
            with open(JobName, 'w') as file:
                file.write(cont)
    
            # Submit the job.
            os.system(f"sbatch {JobName}")


if __name__ == '__main__':
    main()