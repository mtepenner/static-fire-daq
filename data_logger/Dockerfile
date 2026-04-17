FROM golang:1.21-alpine AS builder
WORKDIR /app
COPY go.mod go.sum ./
RUN go mod download
COPY . .
RUN go build -o /logger ./cmd/logger

FROM alpine:3.18
COPY --from=builder /logger /logger
CMD ["/logger"]
