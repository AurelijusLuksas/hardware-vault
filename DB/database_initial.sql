-- Drop and recreate the schema
DROP SCHEMA IF EXISTS hardware_vault CASCADE;
CREATE SCHEMA hardware_vault;

-- 1. Category Table
CREATE TABLE hardware_vault.category (
    id SERIAL PRIMARY KEY, -- Automatic identity
    name VARCHAR(255) UNIQUE NOT NULL,
    description TEXT
);

-- 2. Product Table
CREATE TABLE hardware_vault.product (
    id SERIAL PRIMARY KEY, -- Automatic identity
    category_id INTEGER NOT NULL,
    name VARCHAR(255) UNIQUE NOT NULL,
    description TEXT,
    price DECIMAL(10, 2) NOT NULL CHECK (price >= 0), -- Declarative constraint: price must be non-negative
    stock_quantity INTEGER CHECK (stock_quantity >= 0), -- Declarative constraint: stock_quantity must be non-negative
    FOREIGN KEY (category_id) REFERENCES hardware_vault.category(id)
);

-- 3. Warehouse Table
CREATE TABLE hardware_vault.warehouse (
    id SERIAL PRIMARY KEY, -- Automatic identity
    address VARCHAR(255) UNIQUE NOT NULL,
    organization_id INTEGER NOT NULL,
    FOREIGN KEY (organization_id) REFERENCES hardware_vault.organization(id)
);

-- 4. Employee Table
CREATE TABLE hardware_vault.employee (
    id SERIAL PRIMARY KEY, -- Automatic identity
    position VARCHAR(255),
    years_of_experience INTEGER CHECK (years_of_experience >= 0), -- Declarative constraint: non-negative experience
    contact_info VARCHAR(255),
    warehouse_id INTEGER NOT NULL,
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL,
    FOREIGN KEY (warehouse_id) REFERENCES hardware_vault.warehouse(id)
);

-- 5. Organization Table
CREATE TABLE hardware_vault.organization (
    id SERIAL PRIMARY KEY, -- Automatic identity
    warehouse_count INTEGER DEFAULT 0 CHECK (warehouse_count >= 0) -- Default value and declarative constraint
);

-- 6. Customer Table
CREATE TABLE hardware_vault.customer (
    id SERIAL PRIMARY KEY, -- Automatic identity
    order_count INTEGER DEFAULT 0 CHECK (order_count >= 0), -- Default value and declarative constraint
    contact_info VARCHAR(255),
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL
);

-- 7. Places Order Table
CREATE TABLE hardware_vault.places_order (
    id SERIAL PRIMARY KEY, -- Automatic identity
    customer_id INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    price DECIMAL(10, 2) NOT NULL,
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Default value for timestamps
    count INTEGER CHECK (count > 0), -- Declarative constraint: order count must be positive
    FOREIGN KEY (customer_id) REFERENCES hardware_vault.customer(id),
    FOREIGN KEY (product_id) REFERENCES hardware_vault.product(id)
);

-- 8. Has Product Table
CREATE TABLE hardware_vault.has_product (
    warehouse_id INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    stock_quantity INTEGER CHECK (stock_quantity >= 0),
    PRIMARY KEY (warehouse_id, product_id), -- Composite primary key
    FOREIGN KEY (warehouse_id) REFERENCES hardware_vault.warehouse(id),
    FOREIGN KEY (product_id) REFERENCES hardware_vault.product(id)
);

-- 9. Specification Table
CREATE TABLE hardware_vault.specification (
    id SERIAL PRIMARY KEY, -- Automatic identity
    name VARCHAR(255) NOT NULL,
    description TEXT,
    value VARCHAR(255),
    product_id INTEGER NOT NULL,
    FOREIGN KEY (product_id) REFERENCES hardware_vault.product(id)
);

-- 10. Supplier Table (Updated for One-to-Many Relationship with Product)
CREATE TABLE hardware_vault.supplier (
    id SERIAL PRIMARY KEY, -- Automatic identity
    contact_info VARCHAR(255),
    name VARCHAR(255) UNIQUE NOT NULL,
    rating FLOAT CHECK (rating >= 0 AND rating <= 5), -- Declarative constraint: rating range
    address VARCHAR(255),
    product_id INTEGER NOT NULL, -- Foreign key to product (one-to-many)
    FOREIGN KEY (product_id) REFERENCES hardware_vault.product(id)
);

-- 11. Price Change Log Table (for Trigger)
CREATE TABLE hardware_vault.price_change_log (
    id SERIAL PRIMARY KEY,
    product_id INTEGER NOT NULL,
    old_price DECIMAL(10, 2),
    new_price DECIMAL(10, 2),
    change_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (product_id) REFERENCES hardware_vault.product(id)
);

-- 12. Stock Alert Table (for Trigger)
CREATE TABLE hardware_vault.stock_alert (
    id SERIAL PRIMARY KEY,
    product_id INTEGER NOT NULL,
    alert_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    alert_message TEXT,
    FOREIGN KEY (product_id) REFERENCES hardware_vault.product(id)
);

-- 13. Create Indexes
CREATE UNIQUE INDEX idx_product_name ON hardware_vault.product (name); -- Unique index
CREATE INDEX idx_warehouse_address ON hardware_vault.warehouse (address); -- Non-unique index
CREATE UNIQUE INDEX idx_supplier_contact_info ON hardware_vault.supplier (contact_info); -- Unique index on supplier's contact info
CREATE INDEX idx_product_price ON hardware_vault.product (price); -- Non-unique index on product price

-- Virtual Tables (Views)
-- 1. Products with Low Stock (Stock < 10)
CREATE VIEW hardware_vault.low_stock_products AS
SELECT p.id, p.name, p.stock_quantity
FROM hardware_vault.product p
JOIN hardware_vault.has_product hp ON p.id = hp.product_id
WHERE hp.stock_quantity < 10;

-- 2. Suppliers by Rating (rating > 3.5)
CREATE VIEW hardware_vault.suppliers_by_rating AS
SELECT s.id, s.name, s.rating, s.contact_info
FROM hardware_vault.supplier s
WHERE s.rating > 3.5
ORDER BY s.rating DESC;

-- 3. Active Products (Stock > 0)
CREATE VIEW hardware_vault.active_products AS
SELECT p.id, p.name, p.price, p.stock_quantity
FROM hardware_vault.product p
WHERE p.stock_quantity > 0;

-- 4. High Rated Suppliers (Rating >= 4.0)
CREATE VIEW hardware_vault.high_rated_suppliers AS
SELECT s.id, s.name, s.rating
FROM hardware_vault.supplier s
WHERE s.rating >= 4.0;

-- Materialized View and Refresh
CREATE MATERIALIZED VIEW hardware_vault.product_summary AS
SELECT c.name AS category_name, COUNT(p.id) AS product_count
FROM hardware_vault.product p
JOIN hardware_vault.category c ON p.category_id = c.id
GROUP BY c.name;

-- Refresh Command for Materialized View
REFRESH MATERIALIZED VIEW hardware_vault.product_summary;

-- Materialized View: Product Sales Summary
CREATE MATERIALIZED VIEW hardware_vault.product_sales_summary AS
SELECT p.id AS product_id, p.name AS product_name, SUM(po.count) AS total_sales_quantity, SUM(po.count * po.price) AS total_revenue
FROM hardware_vault.places_order po
JOIN hardware_vault.product p ON po.product_id = p.id
GROUP BY p.id, p.name;

-- Refresh Command for Materialized View
REFRESH MATERIALIZED VIEW hardware_vault.product_sales_summary;


-- Triggers for Business Rules
-- Rule 1: Prevent decreasing stock quantity below zero
CREATE OR REPLACE FUNCTION prevent_negative_stock() RETURNS TRIGGER AS $$
BEGIN
    IF NEW.stock_quantity < 0 THEN
        RAISE EXCEPTION 'Stock quantity cannot be negative';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_prevent_negative_stock
BEFORE UPDATE ON hardware_vault.has_product
FOR EACH ROW EXECUTE FUNCTION prevent_negative_stock();

-- Rule 2: Automatically update organization's warehouse count
CREATE OR REPLACE FUNCTION update_warehouse_count() RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        UPDATE hardware_vault.organization
        SET warehouse_count = warehouse_count + 1
        WHERE id = NEW.organization_id;
    ELSIF TG_OP = 'DELETE' THEN
        UPDATE hardware_vault.organization
        SET warehouse_count = warehouse_count - 1
        WHERE id = OLD.organization_id;
    END IF;
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_update_warehouse_count
AFTER INSERT OR DELETE ON hardware_vault.warehouse
FOR EACH ROW EXECUTE FUNCTION update_warehouse_count();

-- Rule 3: Log Price Change (trigger when product price is updated)
CREATE OR REPLACE FUNCTION log_price_change() RETURNS TRIGGER AS $$
BEGIN
    IF NEW.price <> OLD.price THEN
        INSERT INTO hardware_vault.price_change_log (product_id, old_price, new_price)
        VALUES (NEW.id, OLD.price, NEW.price);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_log_price_change
AFTER UPDATE ON hardware_vault.product
FOR EACH ROW EXECUTE FUNCTION log_price_change();

-- Rule 4: Stock Replenishment Alert (trigger when stock falls below 10)
CREATE OR REPLACE FUNCTION stock_replenishment_alert() RETURNS TRIGGER AS $$
BEGIN
    IF NEW.stock_quantity < 10 THEN
        INSERT INTO hardware_vault.stock_alert (product_id, alert_message)
        VALUES (NEW.id, 'Stock is below 10, consider replenishing.');
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_stock_replenishment_alert
AFTER UPDATE ON hardware_vault.has_product
FOR EACH ROW EXECUTE FUNCTION stock_replenishment_alert();

