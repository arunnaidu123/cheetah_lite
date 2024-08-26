#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys

import setuptools

setup_dir = os.path.dirname(os.path.abspath(__file__))
release_filename = os.path.join(setup_dir, "src", "ska", "c_vhdl_translator", "release.py")
exec(open(release_filename).read())

# prevent unnecessary installation of pytest-runner
needs_pytest = {"pytest", "test", "ptr"}.intersection(sys.argv)
pytest_runner = ["pytest-runner"] if needs_pytest else []

with open('README.md') as readme_file:
    readme = readme_file.read()

setuptools.setup(
    name='cbf-psr-interface',
    version='0.0.2',
    description="",
    long_description=readme + '\n\n',
    url='https://github.com/ska-telescope/cbf-psr-interface',
    packages=setuptools.find_namespace_packages(where="src", include=["ska.*"]),
    package_dir={"": "src"},
    include_package_data=True,
    license="CSIRO Open Source Software Licence Agreement",
    zip_safe=False,
    keywords='cbf-psr-interface',
    entry_points={
        "console_scripts": [
            "translate=ska.c_vhdl_translator.translate:main"
            ]
        },
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Developers',
        'Natural Language :: English',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
    ],
    install_requires=['pygccxml == 1.9.1'],  
    setup_requires=[] + pytest_runner,
    tests_require=[
        'pytest',
        'coverage',
        'pytest-json-report',
        'pytest-forked' ],
)
