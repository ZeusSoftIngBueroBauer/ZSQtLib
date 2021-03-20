import re
import versioneer
from ez_setup import use_setuptools
use_setuptools()
from setuptools import setup, find_packages

from distutils.core import setup

setup(
    name='ZSQtLib',
    #version='5.4.1',
    version=versioneer.get_version(),
    cmdclass=versioneer.get_cmdclass(),
    #packages=['ZSQtLib'],
    packages=find_packages(),
    #package_dir={'': 'src'},
    include_package_data=True,
    package_data={'': ['*.pyd', '*.dll', '*.py']},
    author='ZeusSoft',
    author_email='mailbox@zeussoft.de',
    license=open('LICENSE.txt').read(),
    description="Python site package for ZS Qt Library",
    url='http://www.zeussoft.de',
    long_description=open('README.txt').read(),
    platforms= 'Windows 7',
)
