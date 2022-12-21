from djitellopy import tello
import time
import pygame
import cv2
global img

pygame.init()
win = pygame.display.set_mode((200, 200))

me = tello.Tello()
me.connect()
print(me.get_battery())
me.streamon()

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
    speed = 60
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
    if getKey("f"):
        cv2.imwrite(f'Image/{time.time()}.jpg',img)
        time.sleep(0.2)
    return [lr, fb , ud , yv]
while True :
    vals = keyboardcontrol()
    me.send_rc_control(vals[0],vals[1], vals[2] , vals[3])
    img = me.get_frame_read().frame
    img = cv2.resize(img, (360, 240))
    cv2.imshow("Image", img)
    cv2.waitKey(1)
    time.sleep(0.05)
