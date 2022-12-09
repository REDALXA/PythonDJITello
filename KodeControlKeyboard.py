from djitellopy import tello
from time import sleep
import pygame

pygame.init()
win = pygame.display.set_mode((200,200))

me = tello.Tello()
me.connect()
print(me.get_battery())

def getKey(KeyName) :
    ans = False
    for eve in pygame.event.get() : pass
    keyInput = pygame.key.get_pressed()
    myKey = getattr(pygame,'K_{}'.format(KeyName))
    if keyInput[myKey] :
        ans = True
        pygame.display.update()
    return ans
def keyboardcontrol() :
    lr , fb , ud , yv = 0 , 0 , 0 , 0
    speed = 50
    if getKey("LEFT") : lr = -speed
    elif getKey("RIGHT") : lr = speed
    if getKey("UP") : fb = speed
    elif getKey("DOWN") : fb = -speed
    if getKey("s") : ud = -speed
    if getKey("w") : ud = speed
    if getKey("d") : yv = speed
    elif getKey("a") : yv = -speed
    if getKey("q") : me.takeoff()
    if getKey("z") : me.land()
    return [lr, fb , ud , yv]

me.takeoff()
sleep(0.5)
me.land()
while True :
    vals = keyboardcontrol()
    me.send_rc_control(vals[0],vals[1], vals[2] , vals[3])
    sleep(0.05)
