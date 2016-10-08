"""Design model Decorator component"""
from abc import abstractmethod, ABCMeta


class Component(metaclass=ABCMeta):

    @abstractmethod
    def operation(self):
        pass

