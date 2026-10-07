# kvcache

> This is a key‑value cache service with TTL and periodic tasks for deletion.

## Quick Start 

- Setup `.env` for this use example.env
```.env
PORT=7379 # Port for http 
PERIODIC_SEC=1 # Poll the storage for deletion by ttl.
```

- Routes
- GET `/` - get all - 200 OK
- DELETE `/:kid` - 204 No Content
- GET `/:kid` - get by key - 200 OK
```json
{
    "new": {
        "ttl": "1788763361206478µs", // or null if not set
        "value": "test2"
    }
}
```
- POST `/` - 201 Created
```json
{
    "key": "new",
    "value": {
        "value": "test2",
        "ttl": 5 // now + 5 seconds. This nullable
    }
}
```

## Future

- Switch from HTTP to binary protocol and use client for connect.
- Add auth to connect.
