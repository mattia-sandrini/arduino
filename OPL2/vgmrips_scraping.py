#!/usr/bin python

import os
import io
import shutil
import urllib
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

                # The Content-Disposition doesn't represent correctly utf-8 strings
                # archive_filename = r.headers.get('Content-Disposition').encode('utf-8').decode('utf-8')
                # archive_filename = archive_filename.split('filename=')[1].replace('"', '')

                # Parse the archive name from the url link encoded via URL encoding (% encoding)
                archive_filename = urllib.parse.unquote(download_link[download_link.rfind('/')+1:])
                archive_filename = archive_filename.split('.zip')[0]
                archive_filename = archive_filename[:archive_filename.find('(')].strip('_')
                print('\tDownloading archive "{}"'.format(archive_filename))
                count = 0
                while True:
                    new_folder = '{}{}{}/'.format(download_path, archive_filename, '' if count==0 else '_(%d)'%count)
                    if not os.path.exists(new_folder):
                        os.mkdir(new_folder)
                        break
                    else:
                        count += 1

                archive_content = io.BytesIO(r.content)
                folder_zipfile = ZipFile(archive_content)
                for filename in folder_zipfile.namelist():
                    try:
                        # Make sure to have a lowercase extension after the last '.'
                        dot = filename.rfind('.')
                        if dot > 0:
                            filename.replace(filename[dot:], filename[dot:].lower())

                        if filename[-4:] == '.txt':
                            with open(new_folder+filename, 'wb') as txt_file:
                                txt_file.write(folder_zipfile.read(filename))
                        if filename[-4:] == '.vgz':
                            vgm_filename = filename.replace('.vgz', '.vgm')
                            vgz_content = folder_zipfile.read(filename)
                            vgm_content = gzip.decompress(vgz_content)
                            print('\t\tSaving file: {}'.format(new_folder+vgm_filename))
                            with open(new_folder+vgm_filename, 'wb') as vgm_file:
                                vgm_file.write(vgm_content)
                    except Exception as e:
                        print("\t\tError: "+str(e))
                # end for
        except Exception as e:
            print("\tError: "+str(e))
    # end for
    i += 1
# end while


# Snippet for renaming files in a directory
# root_dir = "audiofiles/VGM/UltimaVI-The_False_Prohpet/"
# for entry in os.listdir(root_dir):
#     os.rename(root_dir+entry, root_dir+entry+".vgm")
