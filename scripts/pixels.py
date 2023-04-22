#!/usr/bin/env python

from PIL import Image
#import matplotlib.pyplot as plt
import can

class Screen:
    def __init__(self):
        im = Image.open("pix.png")
        #convert the image to a pixel array
        self.main_screen = list(im.getdata())
        self.main_screen = [self.main_screen[i:i+im.width] for i in range(0, len(self.main_screen), im.width)]
        self.numbers = []
        self.numbers = self.fill_numbers()
        self.minus = self.fill_minus()
        self.letters = self.fill_letters()
        self.prev_screen = self.main_screen

    def append_bits(x, bits):
        bits = bits.append([int(i) for i in bin(x)[2:]])

    def send_can_msg(self):
        id_num = 256
        bus = can.Bus(interface='socketcan',channel='can0',bitrate=500000,receive_own_messages=True)
        frame_counter = 0
        
        for i in range(len(self.main_screen)):
            j = 0
            while j < len(self.main_screen[0]):
                #calculate 8 bytes for the message
                msg = []
                for k in range(8):
                    binary_string = ''
                    for l in range(3):
                        binary = ''
                        num = self.main_screen[i][j + k][l]
                        for m in range(8):
                            binary = str(num & 1) + binary
                            num = num >> 1
                        binary_string = binary[0] + binary[1] + binary_string
                    binary_string = '00' + binary_string
                    msg.append(int(binary_string, 2))
                #print(msg)
                
                #calculate frame id
                frame_id = id_num + frame_counter
                #frame_id = hex(frame_id)
                #print(frame_id)
                frame_counter += 1
                #send can msg
                can_msg = can.Message(arbitration_id=frame_id, data=msg, is_extended_id=False)
                bus.send(can_msg)
                j += 8


    #def display_screen(self): #for debugging
    #    plt.figure()
    #    plt.imshow(self.main_screen)
    #    plt.show()

    def fill_letters(self): 
        #letters N,S,W,E
        letters = []
        letter_dict = {0:'N',1:'S',2:'W',3:'E'}
        for i in range(4):
            to_open = str(letter_dict[i]) + ".png"
            img = Image.open(to_open)
            pixels = list(img.getdata())
            pixels = [pixels[i:i+img.width] for i in range(0, len(pixels), img.width)]
            letters.append(pixels)
        return letters

    def fill_minus(self):
        #minus character
        img = Image.open("-.png")
        minus = list(img.getdata())
        minus = [minus[i:i+img.width] for i in range(0, len(minus), img.width)]
        return minus

    def fill_numbers(self):
        numbers = []
        #numbers 0-9
        for i in range(10):
            to_open = str(i) + ".png"
            img = Image.open(to_open)
            pixels = list(img.getdata())
            pixels = [pixels[i:i+img.width] for i in range(0, len(pixels), img.width)]
            numbers.append(pixels)
        return numbers

    def change_char(self, x, y, pixels):
        for i in range(len(pixels)):
            for j in range(len(pixels[0])):
                    self.main_screen[x+i][y+j] = pixels[i][j]
    
    def change_dot_state(self,x,y,b):
        if b:
            self.main_screen[x][y] = (255,126,0,255) #make dot visible (and orange)
        else:
            self.main_screen[x][y] = (0,0,0,0) #remove the dot

    def update_temp(self, temp): #przyjmuje dane w postaci zaokraglonego do 1 miejsca po przecinku stringa (ewentualnie z minusem na początku)
        temp_dict = {1: 2, 2: 7, 3: 14}
        row = 2
        self.change_dot_state(7,12,True)
        if temp[1] == '.':
            self.change_char(row,temp_dict[2],self.numbers[int(temp[0])])
            self.change_char(row,temp_dict[3],self.numbers[int(temp[2])])
        elif temp[0] == '-':
            self.change_char(row,temp_dict[1],self.minus)
            self.change_char(row,temp_dict[2],self.numbers[int(temp[1])])
            if temp[2] == '.':
                self.change_char(row,temp_dict[3],self.numbers[int(temp[3])])
            else:
                self.change_dot_state(7,12,False)
                self.change_char(row,temp_dict[3],self.numbers[int(temp[2])])
        else:
            self.change_char(row,temp_dict[1],self.numbers[int(temp[0])])
            self.change_char(row,temp_dict[2],self.numbers[int(temp[1])])
            self.change_char(row,temp_dict[3],self.numbers[int(temp[3])])

    def update_wind(self, speed): #przyjmuje stringa zaokraglonego do jednego miejsca po przecinku
        row = 2
        speed_dict = {1: 40, 2: 47}
        self.change_char(row,speed_dict[1],self.numbers[int(speed[0])]) 
        if speed[1] == '.':
            self.change_dot_state(7,45,True)
            self.change_char(row,speed_dict[2],self.numbers[int(speed[2])])
        else:
            self.change_dot_state(7,45,False)
            self.change_char(row,speed_dict[2],self.numbers[int(speed[1])]) 

    def update_wind_dir(self, dir): #przyjmuje: NE, NW, SE, SW, N, W, E, S 
        dir_dict = {'N':0,'S':1,'W':2,'E':3}
        col_dict = {0:52,1:58}
        row = 2
        dir = dir.split("-")[0]
        if len(dir) == 1:
            self.change_char(row,col_dict[1],self.letters[dir_dict[dir[0]]])
        else:
            self.change_char(row,col_dict[0],self.letters[dir_dict[dir[0]]])
            self.change_char(row,col_dict[1],self.letters[dir_dict[dir[1]]])

    def update_pressure(self, pressure): #przyjmuje w string liczbe calkowita 3 lub 4-cyfrowa
        pressure_dict = {1: 2, 2: 7, 3: 12, 4: 17}
        row = 13
        if len(pressure) == 3:
            for i in range(3):
                self.change_char(row,pressure_dict[i+2],self.numbers[int(pressure[i])])
        elif len(pressure) == 4:
            for i in range(4):
                self.change_char(row,pressure_dict[i+1],self.numbers[int(pressure[i])])

    def update_precipitation(self, precip): #przykladowe dane (string): 55.5, 5.5 (nie zakladam wiekszych niz 99.9)
        precip_dict = {1: 46, 2: 52, 3: 59}
        row = 13
        
        if len(precip) == 3:
            self.change_char(row,precip_dict[2],self.numbers[int(precip[0])])
            self.change_char(row,precip_dict[3],self.numbers[int(precip[2])])
        elif len(precip) == 4:
            self.change_char(row,precip_dict[1],self.numbers[int(precip[0])])
            self.change_char(row,precip_dict[2],self.numbers[int(precip[1])])
            self.change_char(row,precip_dict[3],self.numbers[int(precip[3])])

    def update_date(self, date): #format rrrr-mm-dd
        precip_dict = {1: 3, 2: 8, 3: 15, 4: 20, 5: 27, 6: 32}
        row = 24
        i = 2
        j = 6
        while i < 9:
            self.change_char(row,precip_dict[j],self.numbers[int(date[i+1])])
            j = j - 1
            self.change_char(row,precip_dict[j],self.numbers[int(date[i])])
            j = j - 1
            i = i + 3
            
    def update_time(self, time): #format hh:mm:ss np. 05:03:10
        time_dict = {1: 40, 2: 45, 3: 52, 4: 57}
        row = 24

        self.change_char(row,time_dict[1],self.numbers[int(time[0])])
        self.change_char(row,time_dict[2],self.numbers[int(time[1])])
        self.change_char(row,time_dict[3],self.numbers[int(time[3])])
        self.change_char(row,time_dict[4],self.numbers[int(time[4])])

    def update_screen(self, msg): #msg[data czas, wilgotnosc, cisnienie, temp, kierunek wiatru, sredni wiatr, opady]
        self.prev_screen = self.main_screen

        datetime = msg[0].split(' ')
        self.update_date(datetime[0])
        self.update_time(datetime[1])
        #nie biore pod uwage wilgotnosci
        msg[2] = str(round(float(msg[2])))
        self.update_pressure(msg[2])
        self.update_temp(msg[3])
        if msg[4] != '':
            self.update_wind_dir(msg[4])
        msg[5] = str(round(float(msg[5]), 1))
        self.update_wind(msg[5])
        msg[6] = str(round(float(msg[6]), 1))
        self.update_precipitation(msg[5])

        #if self.prev_screen != self.main_screen:
        try:
            self.send_can_msg()
        except:
            pass

if __name__ == "__main__":

    p = Screen()

    p.update_temp("2.2")
    #for i in range (len(p.main_screen)):
        #for j in range (len(p.main_screen[0])):
            #print(p.main_screen[i][j])
    p.update_wind("16.2")
    p.update_wind_dir("NW-W")
    p.update_pressure("1990")
    p.update_precipitation("23.5")
    p.update_date("2023-01-08")
    p.update_time("05:15:20")

    #p.display_screen()
    p.send_can_msg()