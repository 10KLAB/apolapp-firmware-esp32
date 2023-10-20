import subprocess
import sys

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

print(large_text)
print("""
 >>>>>>>>>>>>>>>>>>>>>>>>>>>--------APOLAPP FIRMWARE UPLOADER v1.0.0-------<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>>>>>>>>>>>>>>>>>>>>>>>>-----------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>>>>>>>>>>>>>>>>>>>>>>>>---------------DEVELOPED BY 10KLAB-------------<<<<<<<<<<<<<<<<<<<<<<<<<
      
      
      """)

port = input("[?] Enter the COM port of your Aolapp programmer (e.g., COM1): ")

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

    # Create a batch file with the upload command
    batch_file_content = f"""esptool --port {port} --baud {baud_rate} write_flash --flash_mode dio --flash_freq 40m --flash_size 4MB 0x10000 {bin_file}
    """

    # Save the batch file
    batch_file_path = "upload_batch.bat"
    with open(batch_file_path, "w") as batch_file:
        batch_file.write(batch_file_content)
    
    


    try:
        # Execute the batch file
        subprocess.run(batch_file_path, shell=True, check=True)
        print("Upload successful.")
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