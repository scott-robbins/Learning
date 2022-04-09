import time
from controller import XboxController
import wireframe
import pygame

W = 500
H = 500

""" KEYBINDINGS """
key_to_function = {
    pygame.K_LEFT: (lambda x: x.translateAll('x', -10)),
    pygame.K_RIGHT: (lambda x: x.translateAll('x', 10)),
    pygame.K_DOWN: (lambda x: x.translateAll('y', 10)),
    pygame.K_UP: (lambda x: x.translateAll('y', -10)),
    pygame.K_EQUALS: (lambda x: x.scaleAll(1.25)),
    pygame.K_MINUS: (lambda x: x.scaleAll(0.8)),
    pygame.K_q: (lambda x: x.rotateAll('X', 0.1)),
    pygame.K_x: (lambda x: x.rotateAll('X', -0.1)),
    pygame.K_a: (lambda x: x.rotateAll('Y', 0.1)),
    pygame.K_d: (lambda x: x.rotateAll('Y', -0.1)),
    pygame.K_w: (lambda x: x.rotateAll('Z', 0.1)),
    pygame.K_s: (lambda x: x.rotateAll('Z', -0.1))}


class ProjectionViewer:
    """ Displays 3D objects on a Pygame screen """
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption('Wireframe Display')
        self.background = (10, 10, 50)

        self.wireframes = {}
        self.displayNodes = True
        self.displayEdges = True
        self.nodeColours = [(255, 255, 255), (255, 0, 0), (0, 255, 0), (0, 0, 255),
                            (255, 0, 255), (0, 255, 255), (255, 255, 0), (0, 128, 255)]
        self.edgeColour = (200, 200, 200)
        self.nodeRadius = 4

    def addWireframe(self, name, wireframe):
        """ Add a named wireframe object. """
        self.wireframes[name] = wireframe

    def run(self):
        """ Create a pygame screen until it is closed. """
        ctl = XboxController()
        running = True
        self.usage()
        while running:
            # check sticks
            lx, ly = ctl.get_left_stick()
            rx, ry = ctl.get_right_stick()
            # check buttons
            (a, b, x, y, lb, rb, bk, start, lstick, rstick) = ctl.get_buttons()
            # press back button to exit
            if bk == 1:
                running = False
            # Left stick rotates the cube
            if abs(ly) > 0.15:
                self.rotateAll('X', 0.1*lx)
                # print(f'Lx:{lx}')
                time.sleep(0.01)
            if abs(lx) > 0.15:
                self.rotateAll('Y', 0.1*ly)
                # print(f'Ly:{ly}')
                time.sleep(0.01)
            if abs(rx) > 0.15:
                # self.translateAll('z', 0.1*ry)
                self.rotateAll('Z', 0.1*rx)
                time.sleep(0.01)

            # D PAD translates the cube
            u, r, d, l = ctl.get_dpad()
            if u == 1:
                self.translateAll('y', -0.0001*H)
            elif r == 1:
                self.translateAll('x', 0.0001*W)
            elif d == 1:
                self.translateAll('y', 0.0001*H)
            elif l == 1:
                self.translateAll('x', -0.0001*W)
            elif rb == 1:
                self.scaleAll(1.0001)
            elif lb == 1:
                self.scaleAll(-0.9)
            # Show the cube
            self.display()
            # Allow user to rotate/translate cube
            pygame.display.flip()

    def display(self):
        """ Draw the wireframes on the screen. """
        # draw the background
        self.screen.fill(self.background)
        # draw edges and nodes
        for wireframe in self.wireframes.values():
            if self.displayEdges:
                for edge in wireframe.edges:
                    pygame.draw.aaline(self.screen, self.edgeColour, (edge.start.x, edge.start.y),
                                       (edge.stop.x, edge.stop.y), 1)
            if self.displayNodes:
                ii = 0
                for node in wireframe.nodes:
                    pygame.draw.circle(self.screen, self.nodeColours[ii], (int(node.x), int(node.y)), self.nodeRadius, 0)
                    ii += 1

    def translateAll(self, axis, d):
        """ Translate all wireframes along a given axis by d units. """
        for wireframe in self.wireframes.values():
            wireframe.translate(axis, d)

    def scaleAll(self, scale):
        """ Scale all wireframes by a given scale, centred on the centre of the screen. """
        centre_x = self.width / 2
        centre_y = self.height / 2
        for wireframe in self.wireframes.values():
            wireframe.scale((centre_x, centre_y), scale)

    def rotateAll(self, axis, theta):
        """ Rotate all wireframe about their centre, along a given axis by a given angle. """
        rotateFunction = 'rotate' + axis
        for wireframe in self.wireframes.values():
            centre = wireframe.findCentre()
            getattr(wireframe, rotateFunction)(centre, theta)

    def usage(self):
        print(' =========================')
        print('|| SIMPLE CUBE SIMULATION ||     ')
        print(' ===========================================================')
        print('\tPRESS the BACK button to QUIT')
        print('')
        print('\tUse the D Pad to Translate the Cube')
        print('')
        print('\tUse the LEFT STICK to rotate cube around X/Y Axis')
        print('')
        print('\tUse the RIGHT STICK to rotate cube around Z Axis')
        print('')
        print('\tLeft and Right Bumper will shrink/scale the Cube')
        print('')
        print('\t\tHave Fun :D')
        print(' ===========================================================')


def main():
    # Create a canvas
    pv = ProjectionViewer(500, 500)
    # Instantiate the cube (including edges/vertices)
    cube = wireframe.Wireframe()
    cube.addNodes([(x, y, z) for x in (10, 90) for y in (10, 90) for z in (10, 90)])
    cube.addEdges(
        [(n, n + 4) for n in range(0, 4)] + [(n, n + 1) for n in range(0, 8, 2)] + [(n, n + 2) for n in (0, 1, 4, 5)])
    # add cube to pygame canvas
    pv.addWireframe('cube', cube)
    # display cube, and make it interactive
    pv.run()


if __name__ == '__main__':
    main()
