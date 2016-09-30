"""Test strategy design mode"""
from abc import ABCMeta, abstractmethod


class Strategy(metaclass=ABCMeta):

    @abstractmethod
    def algorithm(self):
        pass

    @abstractmethod
    def set_cargo(self, cargo_name, price):
        pass

    @abstractmethod
    def get_cargo_price_list(self):
        pass
