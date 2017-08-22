
# coding: utf-8

# NeuralSnap image-to-text poetry generator
# Copyright (C) 2016  Ross Goodwin

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# You may contact Ross Goodwin via email at ross.goodwin@gmail.com or
# address physical correspondence to:

#                     Ross Goodwin c/o ITP
#                     721 Broadway, 4th Floor
#                     New York, NY 10003


# NeuralSnap -- edited
# 
# Works by generating a caption for an image with recurrent and
# convolutional neural networks using NeuralTalk2. That
# (brief) caption is then expanded into a poem using a second
# recurrent neural network.
# 
# Ross Goodwin, 2016

import os
import sys
import subprocess
import json
import re
from string import Template

class ImageNarrator(object):

    def __init__(self, ntalk_model_fp, rnn_model_fp,
                 stanza_len=310):

        self.ntalk_model_fp = ntalk_model_fp
        self.rnn_model_fp = rnn_model_fp

        self.num_images = '4'
        self.stanza_len = str(stanza_len)

        # Don't change these parameters unless
        # you have Karpath's repos cloned in
        # a different location

        self.SCRIPT_PATH = os.getcwd()
        self.NEURALTALK2_PATH = os.path.join(os.getcwd(), '..', 'neuraltalk2')
        self.CHARRNN_PATH = os.path.join(os.getcwd(), '..', 'char-rnn')

        self.expansion_obj_list = list()
        self.caption_list = list()
        self.html_fp = None
        self.url = None

        self.throw_away_text = ['a black and white photo of ', 'a close up of ', 'a black and white picture of ']

    def get_result(self, image_folder_fp, num_images, osc_callback):
        self.expansion_obj_list = list()
        self.caption_list = list()
        self.image_folder_fp = image_folder_fp
        self.num_images = num_images
        self.narrate(osc_callback)

    def narrate(self, osc_callback):
        # NeuralTalk2 Image Captioning

        os.chdir(self.NEURALTALK2_PATH)

        ntalk_cmd_list = [
            'th',
            'eval.lua',
            '-model',
            self.ntalk_model_fp,
            '-image_folder',
            self.image_folder_fp,
            '-num_images',
            self.num_images,
            '-gpuid',
            '-1'
        ]

        print "INIT NEURALTALK2 CAPTIONING"

        ntalk_proc = subprocess.Popen(ntalk_cmd_list)
        ntalk_proc.communicate()


        # Load Captions

        with open(self.NEURALTALK2_PATH+'/vis/vis.json') as caption_json:
            caption_obj_list = json.load(caption_json)

        # RNN Caption Expansion


        os.chdir(self.CHARRNN_PATH)

        print "INIT CHAR-RNN EXPANSION"
        i = 0
        for obj in caption_obj_list:
            caption = obj['caption']

            for text in self.throw_away_text:
                caption = caption.replace(text, '')

            prepped_caption = caption[0].upper() + caption[1:]
            
            temp = '0.5'
            print "EXPANDING AT TEMPERATURE " + temp
            
            rnn_cmd_list = [
                'th',
                'sample.lua',
                self.rnn_model_fp,
                '-length',
                self.stanza_len,
                '-verbose',
                '0',
                '-temperature',
                temp,
                '-primetext',
                prepped_caption,
                '-gpuid',
                '-1'
            ]

            rnn_proc = subprocess.Popen(
                rnn_cmd_list,
                stdout=subprocess.PIPE
            )
            expansion = rnn_proc.stdout.read()
            osc_callback(expansion)

            print expansion
            
            self.expansion_obj_list.append({
                'id': obj['image_id'],
                'text': expansion
            })

        # Back to original working directory
        os.chdir(self.SCRIPT_PATH)

