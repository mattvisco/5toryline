from OSC import OSCServer, OSCClient, OSCMessage
import sys
from time import sleep
from neuralsnap import ImageNarrator

client = OSCClient()
client.connect( ("localhost", 9000) )

server = OSCServer( ("localhost", 12345) )
server.timeout = 0
run = True
n_talk_model = '../models/2016-01-12_neuraltalk2_model_01_rg.t7'
rnn_model = '../models/2016-01-12_char-rnn_model_02_rg.t7'

expander = ImageNarrator(n_talk_model, rnn_model)

# this method of reporting timeouts only works by convention
# that before calling handle_request() field .timed_out is 
# set to False
def handle_timeout(self):
    self.timed_out = True

# funny python's way to add a method to an instance of a class
import types
server.handle_timeout = types.MethodType(handle_timeout, server)

def send_osc(story):
    oscM = OSCMessage('/narration')
    oscM.append(story)
    client.send( oscM )

def narrate_callback(path, tags, args, source):
    expander.get_result(args[0], args[1], send_osc)

server.addMsgHandler( "/narrate_story", narrate_callback )

# user script that's called by the game engine every frame
def each_frame():
    # clear timed_out flag
    server.timed_out = False
    # handle all pending requests then return
    while not server.timed_out:
        server.handle_request()

# simulate a "game engine"
while run:
    # do the game stuff:
    sleep(1)
    # call user script
    each_frame()

server.close()