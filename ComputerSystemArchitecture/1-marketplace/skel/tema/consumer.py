"""
This module represents the Consumer.

Computer Systems Architecture Course
Assignment 1
March 2021
"""

from threading import Thread
from time import sleep


class Consumer(Thread):
    """
    Class that represents a consumer.
    """

    def __init__(self, carts, marketplace, retry_wait_time, **kwargs):
        """
        Constructor.

        :type carts: List
        :param carts: a list of add and remove operations

        :type marketplace: Marketplace
        :param marketplace: a reference to the marketplace

        :type retry_wait_time: Time
        :param retry_wait_time: the number of seconds that a producer must wait
        until the Marketplace becomes available

        :type kwargs:
        :param kwargs: other arguments that are passed to the Thread's __init__()
        """
        super().__init__(**kwargs)
        self.carts = carts
        self.marketplace = marketplace
        self.wait_time = retry_wait_time

    def run(self):
        for cart in self.carts:
            cart_id = self.marketplace.new_cart()
            for action in cart:
                if action['type'] == 'add':
                    for _ in range(action['quantity']):
                        success = self.marketplace.add_to_cart(cart_id, action['product'])
                        while not success:
                            sleep(self.wait_time)
                            success = self.marketplace.add_to_cart(cart_id, action['product'])

                elif action['type'] == 'remove':
                    for _ in range(action['quantity']):
                        self.marketplace.remove_from_cart(cart_id, action['product'])

                else:
                    print(f'ERROR: bad action {action["type"]}')

            products = self.marketplace.place_order(cart_id)
            for product in products:
                print(f'{self.name} bought {product}')
