#!/usr/bin python

import os
import io
import shutil
import requests
import xml.dom.minidom
from lxml import etree
from zipfile import ZipFile
import gzip

url_format = "https://vgmrips.net/packs/chip/ym3812?p={}"
download_path = './audiofiles/VGM/'

i = 0
while True:
    current_url = url_format.format(i)
    print('Downloading page: "{}"\n'.format(current_url))
    r = requests.get(current_url)
    if r.status_code != 200:
        break
    try:
        parser = etree.HTMLParser(recover=True)
        tree = etree.fromstring(r.text, parser)
    except etree.XMLSyntaxError as ex:
        print("\tBeware of syntax errors in the source page: \n{}".format(str(ex)))
    divs = tree.xpath('//div[@class="result row"]')
    if not divs: # If there aren't results within the current page
        break    # Stop the while loop

    for div in divs:
        try:
            #download_elem = tree.xpath('//*[@id="{}"]/div[2]/h2/a[3]'.format(div.attrib['id']))
            download_elem = div.xpath('.//a[@class="download"]')
            if download_elem:
                download_link = download_elem[0].attrib['href']
                r = requests.get(download_link)
                if r.headers.get('Content-Type') != 'application/zip':
                    print("\tThe file has been ignored because it's not a zip archive.")
                    continue
                archive_filename = r.headers.get('Content-Disposition').split('filename=')[1].replace('"', '')
                print('\tDownloading archive "{}"'.format(archive_filename))
                new_folder = '{}{}/'.format(download_path, archive_filename.split('.zip')[0])
                if not os.path.exists(new_folder):
                    os.mkdir(new_folder)

                archive_content = io.BytesIO(r.content)
                folder_zipfile = ZipFile(archive_content)
                for vgz_filename in folder_zipfile.namelist():
                    try:
                        if vgz_filename[-4:] == '.vgz':
                            vgm_filename = vgz_filename.replace('.vgz', '.vgm')
                            vgz_content = folder_zipfile.read(vgz_filename)
                            vgm_content = gzip.decompress(vgz_content)
                            print('\t\tSaving file: {}'.format(new_folder+vgm_filename))
                            with open(new_folder+vgm_filename, 'wb') as vgm_file:
                                vgm_file.write(vgm_content)
                                vgm_file.close()
                    except Exception as e:
                        print("\t\tError: "+str(e))
                # end for
        except Exception as e:
            print("\tError: "+str(e))
    # end for
# end while


# Snippet for remaing files in a directory
# root_dir = "audiofiles/VGM/UltimaVI-The_False_Prohpet/"
# for entry in os.listdir(root_dir):
#     os.rename(root_dir+entry, root_dir+entry+".vgm")
