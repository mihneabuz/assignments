"""
This module represents the Marketplace.

Computer Systems Architecture Course
Assignment 1
March 2021
"""

from threading import Lock
import unittest
import logging

logger = logging.getLogger()
logger.setLevel(logging.INFO)
logger.addHandler(logging.FileHandler(filename='marketplace.log', mode='w'))

class Marketplace:
    """
    Class that represents the Marketplace. It's the central part of the implementation.
    The producers and consumers use its methods concurrently.
    """
    def __init__(self, queue_size_per_producer):
        """
        Constructor

        :type queue_size_per_producer: Int
        :param queue_size_per_producer: the maximum size of a queue associated with each producer
        """
        self.queue_size = queue_size_per_producer
        self.producers = []
        self.producers_lock = Lock()
        self.carts = []
        self.carts_lock = Lock()

    def register_producer(self):
        """
        Returns an id for the producer that calls this.
        """
        self.producers_lock.acquire()
        id = len(self.producers)
        self.producers.append((Lock(), []))
        logger.info(f'Registered producer {id}')
        self.producers_lock.release()
        return id

    def publish(self, producer_id, product):
        """
        Adds the product provided by the producer to the marketplace

        :type producer_id: String
        :param producer_id: producer id

        :type product: Product
        :param product: the Product that will be published in the Marketplace

        :returns True or False. If the caller receives False, it should wait and then try again.
        """
        logger.info(f'Got {product} from producer {producer_id}')
        lock, queue = self.producers[producer_id]

        lock.acquire()
        if len(queue) < self.queue_size:
            queue.append([Lock(), True, product])
            lock.release()
            return True

        else:
            lock.release()
            return False

    def new_cart(self):
        """
        Creates a new cart for the consumer

        :returns an int representing the cart_id
        """
        self.carts_lock.acquire()
        id = len(self.carts)
        self.carts.append([])
        logger.info(f'Registered cart {id}')
        self.carts_lock.release()
        return id

    def add_to_cart(self, cart_id, product):
        """
        Adds a product to the given cart. The method returns

        :type cart_id: Int
        :param cart_id: id cart

        :type product: Product
        :param product: the product to add to cart

        :returns True or False. If the caller receives False, it should wait and then try again
        """
        logger.info(f'Adding {product} to cart {cart_id}')
        cart = self.carts[cart_id]
        
        for producer_id, (_, producer_list) in enumerate(self.producers):
            for prod in producer_list:
                product_lock, product_available, listed_product = prod
                if listed_product == product:
                    product_lock.acquire()
                    if product_available:
                        prod[1] = False
                        cart.append((producer_id, listed_product))
                        product_lock.release()
                        return True
                    else:
                        product_lock.release()

        return False

    def remove_from_cart(self, cart_id, product):
        """
        Removes a product from cart.

        :type cart_id: Int
        :param cart_id: id cart

        :type product: Product
        :param product: the product to remove from cart
        """
        logger.info(f'Removing {product} from cart {cart_id}')
        cart = self.carts[cart_id]

        producer_id = -1
        cart_idx = -1
        for idx, (prod_id, prod) in enumerate(cart):
            if product == prod:
                producer_id = prod_id
                product = prod
                cart_idx = idx

        if producer_id == -1 or cart_idx == -1:
            print("ERROR: bad bad bad")

        _, producer_list = self.producers[producer_id]
        for prod in producer_list:
            product_lock, product_available, listed_product = prod
            if listed_product is product and not product_available:
                product_lock.acquire()
                prod[1] = True
                product_lock.release()

        cart.pop(cart_idx)

    def place_order(self, cart_id):
        """
        Return a list with all the products in the cart.

        :type cart_id: Int
        :param cart_id: id cart
        """
        logger.info(f'Finished cart {cart_id}')
        cart = self.carts[cart_id]

        result = []
        for producer_id, product in cart:
            producer_lock, producer_list = self.producers[producer_id]

            producer_lock.acquire()
            for idx, (product_lock, product_available, listed_product) in enumerate(producer_list):
                if product is listed_product and not product_available:
                    product_lock.acquire()
                    _, _, res = producer_list.pop(idx)
                    result.append(res)
                    product_lock.release()
                    break
            producer_lock.release()
        
        self.carts[cart_id] = ()
        return result



class TestMarketplace(unittest.TestCase):
    # TODO: unit tests
    pass
