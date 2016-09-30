"""Test strategy design mode"""

class Strategy_Context(object):

    def __init__(self, strategy):
        self.__strategy = strategy

    def get_price(self):
        return self.__strategy.algorithm()
