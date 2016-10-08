
from src.component import Component


class ConcreteComponent(Component):

    def operation(self):
        print('Basic Component to be Decorator!')
