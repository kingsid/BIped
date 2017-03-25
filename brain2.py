import cv2 as cv
import time
import numpy as np
import wiringpi2 as wp


pinInterruptW=25
pinLeftW=24
pinRightW=27
pinRightFPV=4
pinLeftFPV=2
pinDownFPV=3
pinUpFPV=17

wp.wiringPiSetupGpio()
wp.pinMode(pinInterruptW,1)
wp.pinMode(pinLeftW,1)
wp.pinMode(pinRightW,1)
wp.pinMode(pinRightFPV,1)
wp.pinMode(pinLeftFPV,1)
wp.pinMode(pinDownFPV,1)
wp.pinMode(pinUpFPV,1)

wp.digitalWrite(pinInterruptW,0)
wp.digitalWrite(pinLeftW,0)
wp.digitalWrite(pinRightW,0)
wp.digitalWrite(pinRightFPV,0)
wp.digitalWrite(pinLeftFPV,0)
wp.digitalWrite(pinDownFPV,0)
wp.digitalWrite(pinUpFPV,0)

Hmin=25#21	#threshold for detecting ball in HSV color space
Hmax=36#57
Smin=120#150
Smax=255#254
Vmin=93#104
Vmax=219#255

end=0

thresh_l=40
thresh_r=140#find
kick_area=2050#find
near_area=1000#find
ball_at_feet=69#find

x=90#fpv initial angles
y=90
cx=-1
cy=-1
area=0.0

rightMax=150
leftMax=30
upMax=110
downMax=68

cam=cv.VideoCapture(0)	#camera adapter and object
cam.set(3,160)	#set frame resolution
cam.set(4,160)

x_centre=80	#camera centre for the bot to align
y_centre=80

def generatePulse(pin):
	wp.digitalWrite(pin,0)#move down
	time.sleep(.01)
	wp.digitalWrite(pin,1)
	time.sleep(.01)
        wp.digitalWrite(pin,0)

def fpv(hor,vert):	#if it moves, return true, else returns false
	global pinRightFPV,pinLeftFPV,pinDownFPV,pinUpFPV,downMax,upMax,rightMax,leftMax
    	global x,y,problem
    	print "X angle: ",x
    	print"y angle",y
   	if hor==0:
		if vert==0:
			print"stable"
		elif vert==1 and y>downMax:
			generatePulse(pinDownFPV)# move down
            		y=y-1
	    		print"y decreased"
			return True
		elif vert==2 and y<upMax:
			generatePulse(pinUpFPV)# move up
            		y=y+1
	    		print"y increased"
			return True
	elif hor==1 and x>leftMax:
			generatePulse(pinLeftFPV)# move left
            		x=x-1
            		print"x decreased"
			return True
	
	elif hor==2 and x<rightMax:
			generatePulse(pinRightFPV)# move left
            		x=x+1
            		print"x increased"
			return True
	return False
def eyes():
	global cx,cy,area
        ret,frame=cam.read()  #capture frame
	HSV=cv.cvtColor(frame,cv.COLOR_BGR2HSV) #convert frame from RGB to HSV
	bin=cv.inRange(HSV,np.array([Hmin,Smin,Vmin]),np.array([Hmax,Smax,Vmax])) #apply threshold and create binary image
	contours, hierarchy = cv.findContours(bin,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE) #find contours in image
	for i in range(0,len(contours)): #for all contours in binary image		
		if cv.contourArea(contours[i]) > 100: #check for area
			a,b,width,height=cv.boundingRect(contours[i]) #draw bounding box
			if abs(width-height)<8 and abs((3.14*width*height)/4 - cv.contourArea(contours[i]))<400: #check if the contour is a circle
                                cx=a+width/2
                                cy=b+height/2
                                area=(3.14*(width)*(height))/4
				print "Ball is present\n"
				print "x coord: ",cx,"\n"
				print "y coord: ",cy,"\n"
				print "width: ",width,"\n"
				print "height: ",height,"\n"
				print "Area: ",(3.14*(width)*(width))/4,"\n"
				return 1
		   
	cx=-1
	cy=-1
	area=0.0
	print "BALL NOT FOUND"
	return 0

# todo update to move camera up down also			    
def search():
	global area,cx,cy,x,y,downMax,upMax,rightMax,leftMax
	flag=0
	if eyes()==1:
		return 1
        while x>leftMax:	
		fpv(1,0)
		flag=eyes()
		print"searching1"
		if flag==1:
			return 1
       # x=30
	print ("end of 1")
        while x<rightMax:
		fpv(2,0)
            	flag=eyes()
	    	print"searching2"
            	if flag==1:
			return 1
       # x=150
	print("end of two")
	fpv(0,1)
	fpv(0,1)
	fpv(0,1)
	fpv(0,1)
	fpv(0,1)
        while x>leftMax:
		fpv(1,0)
            	flag=eyes()
	    	print"searching3"
            	if flag==1:
			return 1
        #x=30
	fpv(0,2)
	fpv(0,2)
	fpv(0,2)
	fpv(0,2)
	fpv(0,2)
        return 0 #nothing found
def walker(left,right):
	global pinLeftW,pinRightW
	wp.digitalWrite(pinLeftW,left)
	wp.digitalWrite(pinRightW,right)

def moveRight():
	global pinInterruptW
	walker(0,1)
	wp.digitalWrite(pinInterruptW,1)
	print "move right"

def moveLeft():
	global pinInterruptW
	walker(1,0)
	wp.digitalWrite(pinInterruptW,1)
	print"move left"

def followArduino():
	global pinInterruptW
	wp.digitalWrite(pinInterruptW,0)

def moveForward():
	global pinInterruptW
	walker(1,1)
	wp.digitalWrite(pinInterruptW,1)

def kick():
	global pinInterruptW
	wp.digitalWrite(pinInterruptW,1)
        walker(0,0)
	time.sleep(10)
	
	
def decide():
    global thresh_r,thresh_l,cx,cy,area,near_area,kick_area,ball_at_feet,x,y,end
    #centre()
    if area<near_area:
		    if x<=thresh_r and x>=thresh_l:
			followArduino()
			print"arduino"
                    elif x>thresh_r:
			moveRight()
                    elif x<thresh_l:
                        moveLeft()
		    else:
			followArduino()
			 
    elif area>=near_area and area<kick_area and y>ball_at_feet:
                    if x>100:
                        moveRight()
			print"moving right to ball"

                    elif x<80:
                        moveLeft()
			print"moving left to ball"

		    else:
			moveForward()
			print"moving ahead"
		
    elif area>=kick_area or y<=ball_at_feet:
		kick()
		print "kicking now"
		end=1
		    
def centre():
        global cx,cy,x_centre,y_centre,x,y,problem,leftMax,rightMax,downMax,upMax
	pixX=30
	pixY=10
	flag=eyes()
	moveAllowed=1
	print"centre called"

        while abs(x_centre-cx)>pixX and x>=leftMax and x<=rightMax and flag==1 and abs(y_centre-cy)>pixY and y>=downMax and y<=upMax:
		if area>kick_area or y<=ball_at_feet:
			kick()
			end=1
			return 1
                if cx>x_centre:
                        moveAllowed=fpv(2,0)
			time.sleep(0.1)
                        flag=eyes()
			print"head right"
			if not moveAllowed:
				moveRight()
				print"moving right to centre"
			else:
				followArduino()	
                elif cx<x_centre:
                        moveAllowed=fpv(1,0)
			time.sleep(0.1)
                        flag=eyes()
			print"head left"
			if not moveAllowed:
				moveLeft()
				print"moving right to centre"
			else:
				followArduino()
		if cy>y_centre:
                        moveAllowed=fpv(0,1)
			time.sleep(0.1)
                        flag=eyes()
			print"head down"
			#if not moveAllowed:
				
				#move bot backward
                elif cy<y_centre:
			moveAllowed=fpv(0,2)
			time.sleep(0.1)
                        flag=eyes()
			print"head up"
			#if not moveAllowed:
				#move bot forward
                
                
	#todo use pid
        while abs(x_centre-cx)>pixX and x>=leftMax and x<=rightMax and flag==1:
		    if area>kick_area or y<=ball_at_feet:
			kick()
			end=1
			return 1
                    if cx>x_centre:
                        moveAllowed=fpv(2,0)
			time.sleep(0.1)
                        flag=eyes()
			print"head right"
			if not moveAllowed:
				moveRight()
				print"moving right to centre"
			else:
				followArduino()	
                    elif cx<x_centre:
                        moveAllowed=fpv(1,0)
			time.sleep(0.1)
                        flag=eyes()
			print"head left"
			if not moveAllowed:
				moveLeft()
				print"moving right to centre"
			else:
				followArduino()
        while abs(y_centre-cy)>pixY and y>=downMax and y<=upMax and flag==1:
		    if area>kick_area or y<=ball_at_feet:
		        kick()
			end=1
			return 1
                    if cy>y_centre:
                        moveAllowed=fpv(0,1)
			time.sleep(0.1)
                        flag=eyes()
			print"head down"
			#if not moveAllowed:
				
				#move bot backward
                    elif cy<y_centre:
			moveAllowed=fpv(0,2)
			time.sleep(0.1)
                        flag=eyes()
			print"head up"
			#if not moveAllowed:
				#move bot forward
	followArduino()
			

while(1):
        
		f=search()
                if f==1:
                        print"f=1"
			temp_area=area #a criterion for threshhold reduction
        		centre()
			if(end==1):
				print"bye"
				break
        		decide()
			if(end==1):
				print"bye"
				break
        		if(area>temp_area):
                    		reduc=((area-temp_area)/area)*(thresh_r-90) #factor to be reduced
                    		thresh_r=thresh_r-reduc
                    		thresh_l=thresh_l+reduc

        		elif(temp_area>area):
		    		inc=((temp_area-area)/temp_area)*(thresh_r-90)
		    		thresh_r=thresh_r+inc
                    		thresh_l=thresh_l-inc
			
		
		cx=-1
		cy=-1
		area=0.0

    
