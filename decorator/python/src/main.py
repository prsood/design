from src.decorator import DecoratorA,DecoratorB
from src.concrete_component import ConcreteComponent

if __name__ == "__main__":
    base = ConcreteComponent()
    decorator_a = DecoratorA()
    decorator_b = DecoratorB()

    decorator_a.set_component(base)
    decorator_b.set_component(decorator_a)

    decorator_b.operation()
