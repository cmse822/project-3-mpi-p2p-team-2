import os
for x in (2**p for p in range(1,10)):
    parameters_string = f"""#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########

#SBATCH --time=04:00:00
#SBATCH --nodes=2         
#SBATCH --ntasks-per-node={int(x/2)}                 
#SBATCH --mem=100MB                    
#SBATCH --job-name RingNonBlocking_{x}
"""

    compilation_string1="""
mpicxx src/RingNonBlocking.cpp -o RingNonBlocking

MessageSize=(2 4 8 16 32 64 128 256 512 1024 2048 4096)
for Size in "${MessageSize[@]}"; do"""
    compilation_string2=f"""
    mpiexec -n "$task" RingNonBlocking  "$Size" "100" data/RingNonBlocking_{int(x)}.csv
done
"""
    with open(f"submitRing_{x}.sb", "w") as file:
        # Writing data to a file
        file.write(parameters_string)
        file.write(compilation_string1)
        file.write(compilation_string2)
    os.system(f'sbatch submitRing_{x}.sb')
