from src.decorator_interface import Decorator


class DecoratorA(Decorator):

    def operation(self):
        super().operation()
        print('DecoratorA modify !')
        print('DecoratorA do more operates')


class DecoratorB(Decorator):

    def operation(self):
        super().operation()
        print('DecoratorB modify!')
        self.__do_something()

    def __do_something(self):
        print('DecoratorB must do something means I did it')
