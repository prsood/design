from src.component import Component


class Decorator(Component):

    def __init__(self):
        self.__component = None

    def operation(self):
        if self.__component is not None:
            self.__component.operation()

    def set_component(self, component):
        self.__component = component
