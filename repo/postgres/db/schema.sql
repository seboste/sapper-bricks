CREATE SCHEMA IF NOT EXISTS entity;
CREATE TABLE IF NOT EXISTS entity.entity
(
    id  text NOT NULL,
    string_property text NOT NULL,
    int_property integer NOT NULL,
    primary key(id)
);
