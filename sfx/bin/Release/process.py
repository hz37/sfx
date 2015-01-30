# Pre-process data file for sound effects utility:
#
# - check if all data files are valid
# - check if every file exists
# - alpha sort data on descriptions
# - remove superfluous spaces
# - turn all descriptions into lowercase
# - toss out non .wav file entries
# - remove doubles
#
# Python 3.4 on Windows 7.
# H.Zimmerman <henszimmerman@gmail.com>, 27-06-2014.

import codecs
import datetime
import os
import re
import time

config_file = 'config.txt'
data_org_file = 'data_org.txt'
data_file = 'data.txt'

# Check if config and unprocessed data file exist.
# If not, exit with an error.

if(not os.path.isfile(config_file)):
    print(config_file + ' not found')
    quit()

if not os.path.isfile(data_org_file):
    print(data_org_file + ' not found')
    quit()

# Read path from config file and check if it is valid.

lines = [line.strip() for line in open(config_file, encoding='latin-1')]

root_directory = lines[0]

print('Root directory: ' + root_directory)

if(not os.path.isdir(root_directory)):
    print('Invalid root directory')
    quit()

# Read entire unsorted data file into a list.

print('reading and checking files...')

counter = 0
d = list()

# Build a list of exact filenames we already got.
# This way we can skip exact doubles that might have sneaked in.

processed_filenames = []

with open(data_org_file, encoding='latin-1') as f:
    for line in f:
        if counter is 0:
            pass
        elif counter % 2 is 1:
            # Remove extraneous spaces from description.
            key = re.sub('[ \t]+', ' ', line)
        else:
            short_filename = line.strip()

            # Check if file exists.
            long_filename = root_directory + short_filename

            if(not os.path.isfile(long_filename)):
                print('Line ' + str(counter * 2 + 1) + ': ' + long_filename + ' does not exist')
                quit()

            # Skip doubles (exist on filename).
            if(not processed_filenames.__contains__(short_filename.lower())):

                # Skip non wav files (they can't be played by current implementation).
                if('.wav' in short_filename.lower()):
                    d.append([key.strip(), short_filename])
                    processed_filenames.append(short_filename.lower())

        counter += 1

# Write the dictionary back to disk sorted.

print('sorting...')

d.sort()

print('writing...')

with open(data_file, 'w', encoding='latin-1') as f:
    # Write machine header.
    time_stamp = time.time()
    datetime_string = datetime.datetime.fromtimestamp(time_stamp).strftime('%Y-%m-%d %H:%M:%S')
    f.write("# process.py generated data: do not edit directly. Updated: " + datetime_string + '\n')

    # Write sfx data.
    for key,value in d:
        f.write(key.lower() + '\n')
        f.write(value + '\n')

print('done!')
