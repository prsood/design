"""inherit from strategy_interface"""
from src.framework.strategy_interface import Strategy


class Offprice(Strategy):

    def __init__(self):
        self.__cargo_list = {}
        pass

    def algorithm(self):
        total_price = 0.0
        for name, price in self.__cargo_list.items():
            total_price += price

        limit= int((total_price/1000))*100 >400 and 400 or int((total_price/1000))*100
        ret_price = total_price >= 1000 and total_price - limit or total_price
        if ret_price == total_price:
            ret_price = total_price > 500.00 and total_price *0.90 or total_price

        print("Total price:%.2f " % total_price)
        return ret_price

    def set_cargo(self, cargo_name, price):
        self.__cargo_list[cargo_name] = price
        print("%s : %.2f" % (cargo_name, price))

    def get_cargo_price_list(self):
        pass