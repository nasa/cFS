core Flgith System (cFS) Unit Test (UT) Assert Library
Open Source Release Readme

UT Assert Release 1.0.0

Date:
June 16, 2015

Introduction

The Unit Test (UT) Assert library provides a unit test framework and a collection 
of utilities that are designed to facilitate unit testing of applications and
components build via the core Flight System (cFS) OSAL/cFE development and 
runtime environment.  

The cFS is a platform and project independent reusable software framework and
set of reusable applications developed by NASA Goddard Space Flight Center.  
This framework is used as the basis for the flight software for satellite data 
systems and instruments, but can be used on other embedded systems.  More 
information on the cFS can be found at http://cfs.gsfc.nasa.gov

Assert-Based Unit Test Tools (UT-Assert) Goals

These tools implement an assertion-based testing philosophy that requires the 
developer to explicitly write verification statements that assert whether a condition 
is true or false. This approach is much different than the Flight Software Branch's 
historical approach to unit testing that creates a log file that requires developer 
analysis in order to determine whether a test passed or failed. In order to use the 
tools a developer populates the framework with their unit tests and links with the 
ut-asssert library to create an executable.

Project Goals

The establishment of the FSW Reuse Library (FSRL) has made it obvious that the 
development and maintenance of unit tests is critical to the success of the FSRL. 
Unit tests are an essential part of the FSRL and they must be developed and 
maintained in a cost effective manner. The unit test framework relieves developers 
from repeatedly implementing common unit testing functions and they enforce a common 
look-and-feel that helps test maintenance. These tools implement an assertion-based 
testing philosophy that requires the developer to explicitly write verification 
statements that assert whether a condition is true or false. This approach is much 
different than the NASG Goddard Space Flight Center Flight Software Branch's 
historical approach to unit testing that creates a log file that requires a developer 
analysis in order to determine whether a test passed or failed.

Functional Goals

This approach allows developers to write self-verifying unit tests and the tools 
provide a framework that allow unit tests to be bundled into a single executional 
units. This aggregation allows comprehensive automated unit testing so as FSRL 
components are added and/or modfiied automated regression unit testing can be 
performed.

Evolvability Goals

Since the core Flight System (cFS) suite is open source another motivation is to 
provide a test facility that can be used by the open source community. 

The use of a third party (including open source) unit test tool such as CUnit was 
considered. However, this option was rejected because it added unnecessary complexity 
to the FSRL unit testing. A tool such as cunit is written to accommodate many different
users which increases its complexity and obscures the basic functionality required by 
the FSRL unit tests. For example, cunit supports the generation of XML reports which 
is not seen as a valuable feature for the FSRL. In addition, little effort is required 
to implement and maintain the unit test framework. Therefore by developing our own 
unit test framework we have complete control over the framework’s features, complexity,
and growth path. We also do not rely on an outside party to implement enhancements 
and/or bug fixes.

Usability Goals

There are multiple aspects to the usability of these tools:

Simplify and 'standardize' the process of creating unit test for CFS software 
components.  Allow developers to create self-documented unit test that ease the 
maintenance of unit tests. Unit test maintenance has proven to be a difficult task.

Allow unit tests to be aggregated into unit test suites so CFS unit test suites can be 
created to allow automated unit level regression testing.

This software is licensed under the NASA Open Source Agreement. 
http://ti.arc.nasa.gov/opensource/nosa

EOF