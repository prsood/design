""" main file for test"""
from src.framework.strategy_context import Strategy_Context
from src.implement.strategy_rebate import Rebate
from src.implement.strategy_offprice import Offprice
from src.implement.strategy_cheap import Cheapprice

if __name__ == "__main__":
    TypeA = Rebate()
    TypeB = Offprice()
    TypeC = Cheapprice()
    print('final price = total * 80%')
    TypeA.set_cargo("Phone", 1999.00)
    TypeA.set_cargo("Battery", 19.00)
    TypeA.set_cargo("Charge Line", 9.99)
    TypeA.set_cargo("Charger", 21.99)
    SCA = Strategy_Context(TypeA)
    print("Final Price:%.2f" % SCA.get_price(), end='\n\n')

    print('if total >1000 decreace 100 per 1000 if >500 and  < 1000 90% ')
    TypeB.set_cargo("NoteBook", 4999.00)
    TypeB.set_cargo("Battery", 219.00)
    TypeB.set_cargo("Computer Bag", 99.99)
    TypeB.set_cargo("USB Mouse", 79.00)
    SCB = Strategy_Context(TypeB)
    print("Final Price:%.2f" % SCB.get_price(), end='\n\n')

    print('if total >1000 decreace 100 per 1000 if >500 and  < 1000 90% ')
    TypeB.set_cargo("NoteBook", 399.00)
    TypeB.set_cargo("Battery", 19.00)
    TypeB.set_cargo("Computer Bag", 99.99)
    TypeB.set_cargo("USB Mouse", 79.00)
    SCB = Strategy_Context(TypeB)
    print("Final Price:%.2f" % SCB.get_price(), end='\n\n')

    print('if total count >=4  delete min price from total')
    TypeC.set_cargo("Bicycle", 899.99)
    TypeC.set_cargo("Ring", 19.30)
    TypeC.set_cargo("Water Bottle", 19.89)
    TypeC.set_cargo("Head Camera", 299.81)
    SCC = Strategy_Context(TypeC)
    print("Final Price:%.2f" % SCC.get_price(), end='\n\n')
