class Dog:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def bark(self):
        print(f"{self.name} says Woof!")
    
    def get_info(self):
        return f"{self.name} is {self.age} years old"

my_dog = Dog("Buddy", 3)
my_dog.bark()
print(my_dog.get_info())