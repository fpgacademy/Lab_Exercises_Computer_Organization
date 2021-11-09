# Zip the design files folder and put them in the release folder

import os, shutil

release_path = './release/'
versions = [['./arm/nano/', 'arm_nano_'], ['./arm/standard/', 'arm_standard_'], ['./niosII/nano/', 'niosII_nano_'], ['./niosII/standard/', 'niosII_standard_']]

for version in versions:
    labs =  os.listdir(version[0])
    for lab in labs:
        design_files_path = version[0] + lab + '/design_files'
        if (os.path.isdir(design_files_path)):
            output_zip_filename = release_path + version[1] + lab + '_design_files'
            shutil.make_archive(output_zip_filename, 'zip', design_files_path);

