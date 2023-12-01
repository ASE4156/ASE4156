CREATE TABLE client (
    client_id SERIAL PRIMARY KEY,
    client_name VARCHAR(50) NOT NULL,
    client_email VARCHAR(255),
    client_password VARCHAR(255) NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE prompt (
    prompt_id SERIAL PRIMARY KEY,
    prompt_name VARCHAR(50) NOT NULL,
    prompt_description VARCHAR(255),
    prompt_content VARCHAR(255) NOT NULL,
    client_id INTEGER REFERENCES client(client_id)
);

CREATE TABLE token (
    token_id VARCHAR(50) PRIMARY KEY,
    created_at TIMESTAMP WITHOUT TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    expires_at TIMESTAMP WITHOUT TIME ZONE DEFAULT (CURRENT_TIMESTAMP + INTERVAL '1 day'),
    client_id INTEGER REFERENCES client(client_id)
);
