#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: async_io.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 23 May 2018 07:35:42 PM PDT

      > Modify Time: 

#########################################################################################
"""

import asyncio
import time
import aiohttp

async def hello():
    print('into...........')
    await asyncio.sleep(1)
    print('exit...........')

loop = asyncio.get_event_loop()
tasks = [hello(), hello()]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
