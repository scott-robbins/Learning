import pygame
import sys
import os

OSYSTEM = os.name
nix_layout = {'A': 0, 'B': 1, 'X': 2, 'Y': 3,
              'LEFT_BUMP': 4, 'LEFT_BUMP': 5,
              'BACK': 6, 'START': 7, 'LEFT_STICK_BTN': 9, 'RIGHT_STICK_BTN': 10,
              'LEFT_STICK_X': 0, 'LEFT_STICK_Y': 1, 'RIGHT_STICK_X': 3, 'RIGHT_STICK_Y': 4,
              'LEFT_TRIGGER': 2, 'RIGHT_TRIGGER': 5}

win_layout = {'A': 0, 'B': 1, 'X': 2, 'Y': 3,
              'LEFT_BUMP': 4, 'RIGHT_BUMP': 5,
              'BACK': 6, 'START': 7, 'LEFT_STICK_BTN': 8, 'RIGHT_STICK_BTN': 9,
              'LEFT_STICK_X': 0, 'LEFT_STICK_Y': 1, 'RIGHT_STICK_X': 3, 'RIGHT_STICK_Y': 4,
              'LEFT_TRIGGER': 2, 'RIGHT_TRIGGER': 5, 'TRIGGERS': -1}

VERSION = int(pygame.version.ver[0])
if VERSION == 3:
    win_layout['RIGHT_STICK_X'] = 4
    win_layout['RIGHT_STICK_Y'] = 3
    win_layout['RIGHT_TRIGGER'] = 2
    win_layout['TRIGGERS'] = 2
LAYOUT = {'nt': win_layout,
          'posix': nix_layout}


class XboxController:
    i = 0
    left_trigger_used = False
    right_trigger_used = False

    def __init__(self):
        pygame.init()
        pygame.joystick.Joystick(self.i).init()
        self.joystick = pygame.joystick.Joystick(self.i)
        self.joystick.init()
        self.dead_zone = 0.15
        self.BUTTONS = LAYOUT[OSYSTEM]
        self.i += 1
        print(f'[+] Xbox Controller {self.i} connected')

    def get_id(self):
        return self.joystick.get_id()


    def get_buttons(self):
        pygame.event.pump()
        return (self.joystick.get_button(self.BUTTONS['A']),
                self.joystick.get_button(self.BUTTONS['B']),
                self.joystick.get_button(self.BUTTONS['X']),
                self.joystick.get_button(self.BUTTONS['Y']),
                self.joystick.get_button(self.BUTTONS['LEFT_BUMP']),
                self.joystick.get_button(self.BUTTONS['RIGHT_BUMP']),
                self.joystick.get_button(self.BUTTONS['BACK']),
                self.joystick.get_button(self.BUTTONS['START']),
                self.joystick.get_button(self.BUTTONS['LEFT_STICK_BTN']),
                self.joystick.get_button(self.BUTTONS['RIGHT_STICK_BTN']))

    def get_left_stick(self):
        pygame.event.pump()
        left_stick_x = self.joystick.get_axis(self.BUTTONS['LEFT_STICK_X'])
        left_stick_y = self.joystick.get_axis(self.BUTTONS['LEFT_STICK_Y'])
        return (left_stick_x, left_stick_y)

    def get_right_stick(self):
        pygame.event.pump()
        right_stick_x = self.joystick.get_axis(self.BUTTONS['RIGHT_STICK_X'])
        right_stick_y = self.joystick.get_axis(self.BUTTONS['RIGHT_STICK_Y'])
        return (right_stick_x, right_stick_y)

    def get_triggers(self):
        trigger_axis = 0.0
        pygame.event.pump()
        left = self.joystick.get_axis(self.BUTTONS['LEFT_TRIGGER'])
        right = self.joystick.get_axis(self.BUTTONS['RIGHT_TRIGGER'])

        if left != 0:
            self.left_trigger_used = True
        if right != 0:
            self.right_trigger_used = True

        if not self.left_trigger_used:
            left = -1
        if not self.right_trigger_used:
            right = -1

        if VERSION != 2 and OSYSTEM == 'nt':
            trigger_axis = -1*self.joystick.get_axis(self.BUTTONS['TRIGGERS'])
        else:
            trigger_axis = (-1 * left + right) / 2
        return trigger_axis

    def get_dpad(self):
        pygame.event.pump()
        xhat, yhat = self.joystick.get_hat(0)
        up = yhat == 1
        down = yhat == -1
        left = xhat == -1
        right = xhat == 1
        return up, right, down, left

