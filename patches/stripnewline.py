import sys
import subprocess

raw_cmd = sys.argv[1].strip()
extra_flags = sys.argv[2].strip()
clean_cmd = raw_cmd.rstrip(' \t\n\r\\')
final_cmd = f"{clean_cmd} {extra_flags}"
subprocess.run(final_cmd, shell=True, check=True)
