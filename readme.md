# grok
Git-Based Package Manager for C++

### how to install
In order to install `grok`, simply run the correct install script for your OS:

- [Linux](https://github.com/fyrware/grok/wiki/Linux-Installation)

Prerequisites:
- [CMake](https://cmake.org/)

### commands
- **`create`:**
  ```shell
  grok create
  ```
  The `create` command is used to create a new package (project). It will ask you a series of questions that it will use 
  to generate a `.grokpackage` file, and then a `.grokmake` file (which is cmake code generated from the package)
  
- **`use`:**
  ```shell
  grok use [<package_name>[, <package_release>]]
  ```
  The `use` command will track down a dependency, save it to the `.grok` folder, and apply it to your project. If a 
  `<package_release>` was provided, then it will also be applied.

- **`update`:**
  ```shell
  grok update [<package_name>[, <package_release>]]
  ```
  The `update` command retrieves updates from the repository of the package and saves them to your `.grok` folder. If a 
  `<package_release>` was provided, then it will also be applied.

- **`apply`:**
  ```shell
  grok apply
  ```
  The `apply` command allows you to regenerate your `.grokmake` file if any changes have occurred that grok is unaware 
  of.

- **`sync`:** 
  ```shell
  grok sync
  ```
  The `sync` command will pull down the registry from grok's source. This updates your local registry to be aware of any 
  potentially new packages that may have been added.
  
### package registry
The registry of packages is [currently] hosted with the source code. If you would like to add your package to the 
registry, simply fork grok, add your `.grokpackage` file, and open a pull-request back into the original repository.

### why another package manager?
This was my first C++ project. It felt like the perfect size and complexity to get my proverbial feet wet. This is not 
intended for production use (though admittedly, I probably will).

### limitations
I currently am not planning to support dependency version mis-matches, like when two dependencies depend on another package, but both want to use a different "release".
