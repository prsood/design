"""inherit from strategy_interface 6217000010040944137"""
from src.framework.strategy_interface import Strategy


class Cheapprice(Strategy):

    def __init__(self):
        self.__cargo_list = {}
        pass

    def algorithm(self):
        total_price = 0.0
        cheap_price = 0.0
        for name, price in self.__cargo_list.items():
            total_price += price
            if cheap_price == 0:
                cheap_price = price
            else:
                cheap_price = cheap_price < price and cheap_price or price
        if len(self.__cargo_list) >= 4:
            cheap_price = total_price - cheap_price
            print("Total price:%.2f " % total_price)
        else:
            cheap_price = total_price
            print("Total price:%.2f , If you buy more then 4 could be more cheaper" % total_price)

        return cheap_price

    def set_cargo(self, cargo_name, price):
        self.__cargo_list[cargo_name] = price
        print("%s : %.2f" % (cargo_name, price))

    def get_cargo_price_list(self):
        pass