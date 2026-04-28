NAME=john-lab
CONTAINER=john-container

build:
	podman build -t $(NAME) .

run:
	podman run -it --name $(CONTAINER) $(NAME)

start:
	podman start -ai $(CONTAINER)

stop:
	-podman stop $(CONTAINER)

rm:
	-podman rm -f $(CONTAINER)

clean:
	-podman rm -f $(CONTAINER)
	-podman rmi -f $(NAME)

restart: stop start

re: clean build run