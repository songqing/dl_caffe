#!/usr/bin/python2.7
#coding=utf-8

import os
import sys
import urllib
import re
import time
import random

class FetchImage:
    def __init__(self):
		self.url_entry = 'http://tieba.baidu.com/f?kw=%C3%C9%C3%E6%C5%AE&ie=utf-8&&pn='
		self.count =7332 
		
    def get_image(self):
        for page_id in range(11, 18):
            print page_id
            page_url = self.url_entry + str(page_id*50-50)
            page_content = urllib.urlopen(page_url).read()
            all_matches = re.findall('href="(/p/[\d]+?)"', page_content)
            #print all_matches
            #return
            for one_url in all_matches:
                #print '@'*10, one_url
                #one_content = urllib.urlopen(one_url).read()
                #phone_urls = re.findall('\'(http://imgsrc.baidu.com/forum/.+?.jpg)\'', one_content)
                phone_urls = 'http://tieba.baidu.com' + one_url
               # for each_phone_url in phone_urls:
                   # print '#'*10, each_phone_url
                image_content = urllib.urlopen(phone_urls).read()
                    #print each_phone_url
                phones_one_page=re.findall('src="(http://imgsrc.baidu.com/forum/.+?.jpg)"', image_content)
                #print phones_one_page
                #return
                for phone_every in phones_one_page:
                    image_every_content = urllib.urlopen(phone_every).read()
                    image_content_save_path = 'graf2/a_' + str(page_id) + '_' + str(self.count) + '.jpg'
                    self.count += 1
                    write_file = open(image_content_save_path, 'wb')
                    write_file.write(image_every_content)
                    write_file.close()
                    sleep_time =1+ 1 * random.random()
                    #time.sleep(sleep_time)

if __name__ ==  '__main__':
    fi = FetchImage()
    fi.get_image()
