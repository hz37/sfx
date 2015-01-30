# Check if configuration of sfx is valid.
# Hens Zimmerman, henszimmerman@gmail.com.
# 09-04-2014.

import os

config_file = 'config.txt'
data_file = 'data.txt'

# Check if config and data file exist.
# If not, exit with an error.

if(not os.path.isfile(config_file)):
    print(config_file + ' does not exist')
    quit()

if(not os.path.isfile(data_file)):
    print(data_file + ' does not exist')
    quit()

# Read path from config file and check if it is valid.

lines = [line.strip() for line in open(config_file)]

root_directory = lines[0]

print('Root directory: ' + root_directory)

if(not os.path.isdir(root_directory)):
    print('Invalid root directory')
    quit()

# Check if every file in data_file exists.

with open(data_file) as f:
    content = f.read().splitlines()

sfx_count = int(len(content) / 2)

all_fine = 1

for idx in range(1, sfx_count):
    filename = root_directory + content[idx * 2]

    # print('Checking: ' + filename)

    if(not os.path.isfile(filename)):
        print('Line ' + str(idx * 2 + 1) + ': ' + filename + ' does not exist')
        all_fine = 0

if(all_fine):
    print('Everything checks out fine!')
else:
    print('Some files are missing!')

