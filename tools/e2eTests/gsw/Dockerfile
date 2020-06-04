FROM python:3.7.7-alpine3.11 

ENV PYTHONUNBUFFERED=1

WORKDIR /test_runner

COPY tools/e2eTests/gsw/gsw.py gsw.py

RUN chmod +x gsw.py

ENTRYPOINT [ "./gsw.py" ]