#!/usr/bin/env python3

import os
import sys
import re
import time
import tkinter as tk
from tkinter import *

class Crit:
    def __init__(self,ID,NAME,LVL,RARITY,RATE):
        self.ID = ID
        self.name = NAME
        self.lvl = int(LVL)
        self.rarity = RARITY
        self.rate = float(RATE)

def breed():
    Lines = []
    Lines = Critsinfo.get('1.0','end').splitlines()
    Crits = []
    Splitted = []
    toBreed = []
    Rarities = ['Common', 'Rare', 'Epic', 'Exotic', 'Legend']
    commands = []
    global BreedCommands
    BreedCommands.delete('1.0','end')
    Critsinfo.delete('1.0','end')

    for index in Lines:
        Splitted = list(filter(None, re.split("[ . \n]", index)))
        if (len(Splitted) == 9):
            Crits.append(Crit(Splitted[0], Splitted[1], Splitted[2], Splitted[3], Splitted[4] + '.' +Splitted[5]))
        else:
            Crits.append(
                Crit(Splitted[0], Splitted[1] + ' ' + Splitted[2], Splitted[3], Splitted[4], Splitted[5]+ '.' +Splitted[6]))
        if (Crits[-1].rate < 4.0) and (Crits[-1].lvl != 30):
            toBreed.append(Crits[-1])
    toBreed.sort(key=lambda x: x.rarity)

    for Rarity in Rarities:
        commands.clear()
        items = [item for item in toBreed if item.rarity == Rarity]
        if len(items) >= 3:
            BreedCommands.insert('end',Rarity +': ' + '\n')
        for index2 in range(len(items)):
            commands.append(items[index2].ID)
            if ((index2+1) % 3) == 0:
                BreedCommands.insert('end', '/breed ' + commands.pop() + ',' + commands.pop() + ',' + commands.pop() + '\n')

root = tk.Tk()

frames = [PhotoImage(file='Miscord.gif',format = 'gif -index %i' %(i)) for i in range(8)]
def update(ind):
    if (ind == 7):
        ind = 0
    frame = frames[ind]
    ind += 1
    GIF.configure(image=frame)
    root.after(200, update, ind)


root.title('Miscord Auto Breeder')
canvas = tk.Canvas(root,height = 500 , width = 1000)
canvas.pack()
BreedCommands = tk.Text(canvas)
BreedCommands.place(relx = 0.78,rely = 0,relheight = 1,relwidth = 0.2)
Critsinfo = tk.Text(canvas)
Critsinfo.place(relx = 0.03,rely =0,relheight = 1,relwidth = 0.65)
BreedButton = tk.Button(canvas,text = 'Breed !',command = lambda : breed())
BreedButton.place(relx = 0.69,rely = 0.4)
Copyrights = tk.Label(canvas,text = 'by thomato')
Copyrights.place(relx = 0.68,rely = 0.95)
GIF = Label(root)
GIF.place(relx = 0.68,rely = 0.7,relwidth = 0.1)
root.after(0, update, 0)
root.mainloop()
