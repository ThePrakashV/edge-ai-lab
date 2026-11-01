#Single Inheritance
"""
class Animal:
    def eat(self):
        print("Animal eats food")

class Dog(Animal):
    def bark(self):
        print("Dog barks")

d = Dog()
d.eat()
d.bark()"""
#Multilevel Inheritance
'''
class Animal:
    def eat(self):
        print("Animal eats food")

class Dog(Animal):
    def bark(self):
        print("Dog barks")

class Puppy(Dog):
    def weep(self):
        print("Puppy weeps")

p = Puppy()
p.eat()
p.bark()
p.weep()
'''
#Hierarchical Inheritance
'''
class Animal:
    def eat(self):
        print("Animal eats food")

class Dog(Animal):
    def bark(self):
        print("Dog barks")

class Cat(Animal):
    def meow(self):
        print("Cat meows")

d = Dog()
c = Cat()
d.eat(); d.bark()
c.eat(); c.meow()
'''
#Multiple Inheritance
'''
class Swimmer:
    def swim(self):
        print("Can swim")

class Flyer:
    def fly(self):
        print("Can fly")

class Duck(Swimmer, Flyer):
    def quack(self):
        print("Duck quacks")

dk = Duck()
dk.swim()
dk.fly()
dk.quack()
'''
#Hybrid Inheritance
'''
class Animal:
    def eat(self):
        print("Animal eats food")

class Swimmer:
    def swim(self):
        print("Can swim")

class Dog(Animal):
    def bark(self):
        print("Dog barks")

class Pet(Dog, Swimmer):
    def play(self):
        print("Pet plays")

p = Pet()
p.eat()
p.bark()
p.swim()
p.play()
'''