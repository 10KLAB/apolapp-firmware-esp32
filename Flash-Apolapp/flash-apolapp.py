import subprocess
import sys
import os
import venv




def is_esptool_installed():
    try:
        subprocess.run('esptool --version', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        return True
    except subprocess.CalledProcessError:
        return False
  
def install_esptool():
    # if not is_esptool_installed():
    try:
        subprocess.run('python -m pip install esptool', shell=True, check=True)
        print("esptool has been installed.")
    except subprocess.CalledProcessError as e:
        print(f"Failed to install esptool: {e}")
    # else:
    #     print("esptool is already installed.")

def get_esptool_path():
    try:
        # Define the command to run to locate esptool
        esptool_cmd = 'where esptool'

        # Run the command to locate esptool
        result = subprocess.run(esptool_cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        if result.returncode == 0:
            # esptool found, return its path
            output_lines = result.stdout.split('\n')
            esptool_path = output_lines[0].strip()  # Get the first line and remove leading/trailing whitespace
            return esptool_path

        # esptool not found, so try esptool.py
        esptool_cmd = 'where esptool.py'
        result = subprocess.run(esptool_cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        if result.returncode == 0:
            # esptool.py found, return its path
            output_lines = result.stdout.split('\n')
            esptool_path = output_lines[0].strip()
            return esptool_path

        # Neither esptool nor esptool.py found
        print("No path to 'esptool' or 'esptool.py' found.")
        return None

    except Exception as e:
        print(f"An error occurred: {e}")
        return None


large_text = """
                                                                                                                                                                                                               
                            ###############################################     
                            ###                                         ###     
                            ###      ###       #####      ####    ####  ###     
                            ###  ######     ###########   ####  ######  ###     
                            ###  ####      #############  #### #####    ###     
                            ###       ###  ###      ####  #########     ###     
                            ###      #### ####       ###  #######       ###     
                            ###      ####  ###      ####  ######        ###     
                            ###      ####  ####     ####  ##### ###     ###     
                            ###      ####   ###     ###   ####   ####   ###     
                            ###      ####                 ####     ###  ###     
                            ###                                         ###     
                            ###############################################     
                            ###############################################     
                            ####  ############### #########          ######     
                            ####  ##############  #########  #######  #####     
                            ####  #############  ##########  ######  ######     
                            ####  ############  ####  #####           #####     
                            ####  ############  #####  ####  ########  ####     
                            ####  ###########          ####  ########  ####     
                            ####  ##     ###  ########  ###  ####     #####     
                            ###############################################     
                            ###############################################     
                                                                                                                               
"""

install_esptool()
print(large_text)
print("""
 >>>>>>>>>>>>>>>>>>>>>>>>>>>--------APOLAPP FIRMWARE UPLOADER v1.0.0-------<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>>>>>>>>>>>>>>>>>>>>>>>>-----------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>>>>>>>>>>>>>>>>>>>>>>>>---------------DEVELOPED BY 10KLAB-------------<<<<<<<<<<<<<<<<<<<<<<<<<
      
      
      """)







port = input("[?] Enter the COM port of your Apolapp programmer (e.g., COM1): ")

print('\n[!] 1)Press the "EN" and "BOOT" buttons simultaneously for a few seconds.')
print('[!] 2)Release the "EN" button and continue pressing the "BOOT" button for a few seconds.')
print('[!] 3)When the firmware loading begins, release the "BOOT" button.\n')
start = input("[?] Do you want to start the upload? (y/n): ").strip().lower()
if start != "y":
    sys.exit()

first_message = True


def upload_firmware():

    # Replace these variables with your own values
    baud_rate = 460800  # The baud rate of your ESP
    bin_file = "firmware.bin"  # Replace with the path to your .bin file
    esptool_path = get_esptool_path()
    # Attempt the first command (esptool.py)
    try:
        subprocess.run(f'"{esptool_path}" --port {port} --baud {baud_rate} write_flash --flash_mode dio --flash_freq 40m --flash_size 4MB 0x10000 {bin_file}', shell=True, check=True, stderr=subprocess.DEVNULL)
        print("Upload successful")
    except subprocess.CalledProcessError as e:
        print(f"Upload failed: {e}")



while True:
    upload_firmware()
    
    # Ask if the user wants to upload to another ESP
    print('\n[!] 1)Press the "EN" and "BOOT" buttons simultaneously for a few seconds.')
    print('[!] 2)Release the "EN" button and continue pressing the "BOOT" button for a few seconds.')
    print('[!] 3)When the firmware loading begins, release the "BOOT" button.\n')
    
    another_upload = input("[?] Do you want to upload firmware to another ESP? (y/n): ").strip().lower()
    if another_upload != "y":
        break
