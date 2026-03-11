#!/usr/bin/env python3
import os
from Crypto.PublicKey import RSA

os.makedirs("certs", exist_ok=True)

key = RSA.generate(1024)
open("certs/debug", "wb").write(key.export_key())
open("certs/debug.pub", "wb").write(key.publickey().export_key())
open("certs/release.pub", "wb").write(key.publickey().export_key())

print("Generated keypair in certs/")
