"""inherit from strategy_interface"""
from src.framework.strategy_interface import Strategy


class Rebate(Strategy):

    def __init__(self):
        self.__cargo_list = {}
        self.offprice = 80;

    def algorithm(self):
        total_price = 0.0
        for name, price in self.__cargo_list.items():
            total_price += price
        rebate = total_price * self.offprice / 100.00
        print("Total price:%.2f " % total_price)
        return rebate

    def set_cargo(self, cargo_name, price):
        self.__cargo_list[cargo_name] = price
        print("%s : %.2f" % (cargo_name, price))

    def get_cargo_price_list(self):
        pass
