.PHONY: flash-firmware up down build logs

flash-firmware:
	cd firmware && pio run --target upload

up:
	docker-compose up -d

down:
	docker-compose down

build:
	docker-compose build

logs:
	docker-compose logs -f
