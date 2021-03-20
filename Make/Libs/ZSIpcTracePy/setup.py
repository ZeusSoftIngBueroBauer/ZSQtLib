from setuptools import setup, find_packages
from distutils.core import Extension
import pathlib

here = pathlib.Path(__file__).parent.resolve()

long_description = (here / 'README.md').read_text(encoding='utf-8')

zsipctrace_module = Extension('ZSIpcTracePy', sources = ['../../../Source/Libs/ZSIpcTracePy/ZSIpcTrcPyDllMain.cpp'])

setup(
    name = 'zsipctrace',
    version = '4.6.3',
    description = 'Python Package with ZSIpcTrace C++ extension',
    long_description=long_description,
    author='ZeusSoft, Ing. Buero Bauer',
    author_email='Christian.Bauer@ZeusSoft.de',
    classifiers=[
       'Development Status :: 3 - Alpha',
       'Intended Audience :: Developers',
       'Topic :: Software Development',
       'License :: OSI Approved :: MIT License',
       'Programming Language :: Python :: 3',
       'Programming Language :: Python :: 3.5',
       'Programming Language :: Python :: 3.6',
       'Programming Language :: Python :: 3.7',
       'Programming Language :: Python :: 3.8',
       'Programming Language :: Python :: 3 :: Only',
    ],
    keywords='development, tracing, debug, multi threaded programming',
    packages=find_packages(),
    python_requires='>=3.5, <4',
    ext_modules=[zsipctrace_module]
)
