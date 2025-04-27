import re
from datetime import datetime
import os

def read_version_from_file(version_file):
    with open(version_file, 'r') as file:
        version = file.readline().strip()
        return version

def parse_version(version_string):
    # Match version pattern with optional suffix like '-beta'
    version_match = re.match(r'(\d+)\.(\d+)\.(\d+)\.(\d+)(.*)', version_string)
    if version_match:
        major = version_match.group(1)
        minor = version_match.group(2)
        patch = version_match.group(3)
        build = version_match.group(4)
        suffix = version_match.group(5).strip('-')
        return major, minor, patch, build, suffix
    return None
def generate_resource_script(version):
    major, minor, patch, build, suffix = parse_version(version)
    current_year = datetime.now().year
    file_version = f"{major},{minor},{patch},{build}"
    product_version = file_version

    resource_script = f'''
#define IDI_APP_ICON 101

IDI_ICON1 ICON "icons/icon.ico"

1 VERSIONINFO
  FILEVERSION {file_version}
  PRODUCTVERSION {product_version}
  FILEOS 0x4
  FILETYPE 0x0
BEGIN
  BLOCK "StringFileInfo"
  BEGIN
    BLOCK "040904B0"
    BEGIN
      VALUE "CompanyName", "Weespin"
      VALUE "FileDescription", "Khinsider Downloader"
      VALUE "FileVersion", "{file_version}"
      VALUE "InternalName", "KhinsiderDownloader"
      VALUE "LegalCopyright", "Copyright © {current_year} Weespin"
      VALUE "OriginalFilename", "KhinsiderDownloader.exe"
      VALUE "ProductName", "Khinsider Downloader"
      VALUE "ProductVersion", "{product_version}"
    END
  END 
  
  BLOCK "VarFileInfo"
  BEGIN
    VALUE "Translation", 0x0409, 0x04B0
  END
END
'''

    return resource_script

def main():
    version_file = os.path.join(os.path.dirname(__file__), '..', '..', 'APP_VERSION')
    version = read_version_from_file(version_file)
    
    if version:
        resource_path = os.path.join(os.path.dirname(__file__), '..', '..', 'resource.rc')
        resource_script = generate_resource_script(version)
        # Save the generated resource script to 'resource.rc'
        with open(resource_path, 'w') as file:
            file.write(resource_script)
        print("Resource script saved to 'resource.rc'.")
    else:
        print("Version file is empty or invalid.")

if __name__ == '__main__':
    main()
