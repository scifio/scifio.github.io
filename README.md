# The [SCIFIO Website](https://scif.io)

This is the repository for the [SCIFIO website](https://scif.io). It's a simple front-facing website that lists basic information about SCIFIO and directs users to more information if they need it. In addition, it has a few useful image resources for users. It is built with [Bootstrap 4](https://getbootstrap.com), [jQuery](http://jquery.com/), [Font Awesome](https://fontawesome.com), and a few other open-source web libraries.

Like SCIFIO itself, this website is built by the SCIFIO community, and is continuously improved by contributors all around the world. With that in mind, all forks and PRs are welcome!

## Development Setup

Setting up the website and developing on it is quite simple - all any developer needs is terminal access and a text editor!

First, clone (and fork, if needed) the repository:

```sh
$ git clone https://github.com/fiji/fiji.github.io.git
$ cd fiji.github.io
```

*You will most likely have to serve this directory with an HTTP server.* This is because all of the site resources (CSS, JS, some images) are hardlinked from root - opening up `index.html` with just your browser will often error.  Luckily, there are many ways to start a quick webserver - here is one example with python.

```sh
$ python -m SimpleHTTPServer
Serving HTTP on 0.0.0.0 port 8000 ...
```

And voila, everything should be done! Visit whatever port the HTTP server is opened on (this tends to be `http://localhost:8000/` by default).