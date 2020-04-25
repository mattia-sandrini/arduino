#!/usr/bin python

import os
import io
import shutil
import requests
import xml.dom.minidom
from lxml import etree
from zipfile import ZipFile
import gzip


from selenium import webdriver
from selenium.webdriver.chrome.options import Options

url = "https://vgmrips.net/packs/chip/ym3812?p={}"

download_path = './audiofiles/VGM/'

# Define and create a temporary donwload folder
download_temp_path = './download_temp/'
if not os.path.exists(download_temp_path):
    os.mkdir(download_temp_path)


i = 0
while True:
    r = requests.get(url.format(i))

    if r.status_code == 200:
        try:
            parser = etree.HTMLParser(recover=True)
            tree = etree.fromstring(r.text, parser)
        except etree.XMLSyntaxError as ex:
            print("Beware of syntax errors in the source page: \n{}".format(str(ex)))
        divs = tree.xpath('//div[@class="result row"]')
        for div in divs:
            print('Div ID: {}'.format(div.attrib['id']))
            #download_elem = tree.xpath('//*[@id="{}"]/div[2]/h2/a[3]'.format(div.attrib['id']))
            download_elem = div.xpath('.//a[@class="download"]')
            if download_elem:
                download_link = download_elem[0].attrib['href']
                print('Download link: {}'.format(download_link))
                r = requests.get(download_link)
                if r.headers.get('Content-Type') != 'application/zip':
                    print("The file has been ignored because not not a zip archive.")
                    continue
                archive_filename = r.headers.get('Content-Disposition').split('filename=')[1].replace('"', '')
                print(archive_filename)
                new_folder = '{}{}/'.format(download_path, archive_filename.split('.zip')[0])
                if not os.path.exists(new_folder):
                    os.mkdir(new_folder)
                #open(download_temp_path+archive_filename, 'wb').write(r.content)

                archive_content = io.BytesIO(r.content)
                folder_zipfile = ZipFile(archive_content)
                for vgz_filename in folder_zipfile.namelist():
                    print(vgz_filename)
                    if vgz_filename[-4:] == '.vgz':
                        vgm_filename = vgz_filename.replace('.vgz', '.vgm')
                        vgz_content = folder_zipfile.read(vgz_filename)
                        vgm_content = gzip.decompress(vgz_content)
                        print(new_folder+vgm_filename)
                        with open(new_folder+vgm_filename, 'wb') as vgm_file:
                            vgm_file.write(vgm_content)
                            vgm_file.close()

                        # vgz_content = io.BytesIO(folder_zipfile.read(vgz_filename))
                        # vgz_zipfile = ZipFile(vgz_content)
                        # vgm_filename = vgz_zipfile.namelist()[0]
                        # vgm_content = vgz_zipfile.read(vgm_filename)
                        # if '.vgm' not in vgm_filename:
                        #     vgm_filename += '.vgm'     # Append the file extension
                        #
                        # new_folder = download_path+archive_filename.split('.zip')[0]
                        # if not os.path.exists(new_folder):
                        #     os.mkdir(new_folder)
                        #
                        # print(new_folder+vgm_filename)
                        # f = open(new_folder+vgm_filename, 'wb')
                        # f.write(vgm_content)
                        # f.close()
            break
    else:
        break
    break
    i += 1


# Delete the temporary download folder
shutil.rmtree(download_temp_path, ignore_errors=True)

# root_dir = "audiofiles/VGM/UltimaVI-The_False_Prohpet/"
# for entry in os.listdir(root_dir):
#     os.rename(root_dir+entry, root_dir+entry+".vgm")

