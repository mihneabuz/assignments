"""
This module represents the Producer.

Computer Systems Architecture Course
Assignment 1
March 2021
"""

from threading import Thread
from time import sleep
from copy import copy

class Producer(Thread):
    """
    Class that represents a producer.
    """

    def __init__(self, products, marketplace, republish_wait_time, **kwargs):
        """
        Constructor.

        @type products: List()
        @param products: a list of products that the producer will produce

        @type marketplace: Marketplace
        @param marketplace: a reference to the marketplace

        @type republish_wait_time: Time
        @param republish_wait_time: the number of seconds that a producer must
        wait until the marketplace becomes available

        @type kwargs:
        @param kwargs: other arguments that are passed to the Thread's __init__()
        """
        super().__init__(**kwargs)
        self.products = products
        self.marketplace = marketplace
        self.id = marketplace.register_producer()
        self.wait_time = republish_wait_time

    def run(self):
        while True:
            for product, quantity, duration in self.products:
                for _ in range(quantity):
                    sleep(duration)
                    success = self.marketplace.publish(self.id, copy(product))
                    while not success:
                        sleep(self.wait_time)
                        success = self.marketplace.publish(self.id, copy(product))
