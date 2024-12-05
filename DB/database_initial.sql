-- Drop and recreate the schema
-- Drop Triggers
DROP TRIGGER IF EXISTS trg_prevent_negative_stock ON hardware_vault.has_product;
DROP TRIGGER IF EXISTS trg_update_warehouse_count ON hardware_vault.warehouse;
DROP TRIGGER IF EXISTS trg_log_price_change ON hardware_vault.product;
DROP TRIGGER IF EXISTS trg_update_order_count ON hardware_vault.places_order;
DROP TRIGGER IF EXISTS trg_stock_replenishment_alert ON hardware_vault.has_product;
DROP TRIGGER IF EXISTS set_order_price ON hardware_vault.places_order;
DROP TRIGGER IF EXISTS before_place_order ON hardware_vault.places_order;
DROP TRIGGER IF EXISTS after_replenish_stock ON hardware_vault.product;
DROP TRIGGER IF EXISTS trg_check_product_exists ON hardware_vault.specification;
DROP TRIGGER IF EXISTS trg_check_warehouse_exists ON hardware_vault.employee;
DROP TRIGGER IF EXISTS before_insert_customer ON hardware_vault.customer;

-- Drop Functions
DROP FUNCTION IF EXISTS prevent_negative_stock();
DROP FUNCTION IF EXISTS update_warehouse_count();
DROP FUNCTION IF EXISTS log_price_change();
DROP FUNCTION IF EXISTS update_order_count();
DROP FUNCTION IF EXISTS stock_replenishment_alert();
DROP FUNCTION IF EXISTS calculate_order_price();
DROP FUNCTION IF EXISTS handle_order_stock();
DROP FUNCTION IF EXISTS replenish_stock();
DROP FUNCTION IF EXISTS check_product_exists();
DROP FUNCTION IF EXISTS check_warehouse_exists();
DROP FUNCTION IF EXISTS set_order_count_to_zero();

-- Drop Materialized Views
DROP MATERIALIZED VIEW IF EXISTS hardware_vault.product_summary;
DROP MATERIALIZED VIEW IF EXISTS hardware_vault.product_stock_summary;
DROP MATERIALIZED VIEW IF EXISTS hardware_vault.all_product_revenue;

-- Drop Views
DROP VIEW IF EXISTS hardware_vault.low_stock_products;
DROP VIEW IF EXISTS hardware_vault.suppliers_by_rating;
DROP VIEW IF EXISTS hardware_vault.active_products;
DROP VIEW IF EXISTS hardware_vault.high_rated_suppliers;
DROP VIEW IF EXISTS hardware_vault.low_stock_products_in_warehouse;
DROP VIEW IF EXISTS hardware_vault.lowest_delivery_price;

-- Drop Indexes
DROP INDEX IF EXISTS idx_employee_fullname;
DROP INDEX IF EXISTS idx_customer_fullname;
DROP INDEX IF EXISTS idx_product_name;
DROP INDEX IF EXISTS idx_category_name;
DROP INDEX IF EXISTS idx_warehouse_address;
DROP INDEX IF EXISTS idx_supplier_contact_info;
DROP INDEX IF EXISTS idx_stock_quantity;
DROP INDEX IF EXISTS idx_supplier_rating;
DROP INDEX IF EXISTS idx_product_price;

-- Drop Tables
DROP TABLE IF EXISTS hardware_vault.price_change_log;
DROP TABLE IF EXISTS hardware_vault.stock_alert;
DROP TABLE IF EXISTS hardware_vault.supplier;
DROP TABLE IF EXISTS hardware_vault.specification;
DROP TABLE IF EXISTS hardware_vault.has_product;
DROP TABLE IF EXISTS hardware_vault.places_order;
DROP TABLE IF EXISTS hardware_vault.customer;
DROP TABLE IF EXISTS hardware_vault.employee;
DROP TABLE IF EXISTS hardware_vault.warehouse;
DROP TABLE IF EXISTS hardware_vault.organization;
DROP TABLE IF EXISTS hardware_vault.product;
DROP TABLE IF EXISTS hardware_vault.category;


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
    stock_quantity INTEGER CHECK (stock_quantity >= 0), -- Declarative constrasint: stock_quantity must be non-negative
    FOREIGN KEY (category_id) REFERENCES hardware_vault.category(id)
);

-- 3. Organization Table
CREATE TABLE hardware_vault.organization (
    id SERIAL PRIMARY KEY, -- Automatic identity
    warehouse_count INTEGER DEFAULT 0 CHECK (warehouse_count >= 0) -- Default value and declarative constraint
);

-- 4. Warehouse Table
CREATE TABLE hardware_vault.warehouse (
    id SERIAL PRIMARY KEY, -- Automatic identity
    address VARCHAR(255) UNIQUE NOT NULL,
    organization_id INTEGER NOT NULL,
    FOREIGN KEY (organization_id) REFERENCES hardware_vault.organization(id)
);

-- 5. Employee Table
CREATE TABLE hardware_vault.employee (
    id SERIAL PRIMARY KEY, -- Automatic identity
    position VARCHAR(255),
    years_of_experience INTEGER CHECK (years_of_experience >= 0), -- Declarative constraint: non-negative experience
    contact_info VARCHAR(255),
    warehouse_id VARCHAR(255) NOT NULL,
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL,
    FOREIGN KEY (warehouse_id) REFERENCES hardware_vault.warehouse(address)
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
    phone_number VARCHAR(255),
    email VARCHAR(255),
    name VARCHAR(255) UNIQUE NOT NULL,
    rating FLOAT CHECK (rating >= 0 AND rating <= 5), -- Declarative constraint: rating range
    address VARCHAR(255),
    product_id INTEGER NOT NULL, -- Foreign key to product (one-to-many)
    delivery_price DECIMAL(10, 2) CHECK (delivery_price >= 0), -- Declarative constraint: non-negative delivery price
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
CREATE INDEX idx_employee_fullname ON hardware_vault.employee (first_name, last_name); -- Composite index
CREATE INDEX idx_customer_fullname ON hardware_vault.customer (first_name, last_name); -- Composite index

CREATE UNIQUE INDEX idx_product_name ON hardware_vault.product (name);
CREATE UNIQUE INDEX idx_category_name ON hardware_vault.category (name);
CREATE UNIQUE INDEX idx_warehouse_address ON hardware_vault.warehouse (address);
CREATE UNIQUE INDEX idx_supplier_contact_info ON hardware_vault.supplier (phone_number, email); 

CREATE INDEX idx_stock_quantity ON hardware_vault.product (stock_quantity);
CREATE INDEX idx_supplier_rating ON hardware_vault.supplier (rating);
CREATE INDEX idx_product_price ON hardware_vault.product (price);

-- Virtual Tables (Views)
-- 1. Products with Low Stock (Stock < 10)
CREATE VIEW hardware_vault.low_stock_products AS
SELECT p.id, p.name, p.stock_quantity
FROM hardware_vault.product p
WHERE p.stock_quantity < 10;

-- 2. Suppliers by Rating (rating > 3.5)
CREATE VIEW hardware_vault.suppliers_by_rating AS
SELECT s.id, s.name, s.rating, s.phone_number, s.email
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

-- 5. Low Stock Products in Warehouse (Stock < 10)
CREATE VIEW hardware_vault.low_stock_products_in_warehouse AS
SELECT hp.warehouse_id, hp.product_id, p.name, p.stock_quantity
FROM hardware_vault.has_product hp
JOIN hardware_vault.product p ON hp.product_id = p.id
WHERE p.stock_quantity < 10;

-- 6. Lowest Delivery Price by Supplier
CREATE VIEW hardware_vault.lowest_delivery_price AS
SELECT s.id, s.name, s.delivery_price, p.name AS product_name
FROM hardware_vault.supplier s
LEFT JOIN hardware_vault.product p ON s.product_id = p.id
WHERE s.delivery_price = (SELECT MIN(delivery_price) FROM hardware_vault.supplier);

-- Materialized View and Refresh
CREATE MATERIALIZED VIEW hardware_vault.product_summary AS
SELECT c.name AS category_name, COUNT(p.id) AS product_count
FROM hardware_vault.product p
JOIN hardware_vault.category c ON p.category_id = c.id
GROUP BY c.name;

-- Refresh Command for Materialized View
REFRESH MATERIALIZED VIEW hardware_vault.product_summary;

-- Materialized View: Product Sales Summary
CREATE MATERIALIZED VIEW hardware_vault.product_stock_summary AS
SELECT p.id AS product_id, p.name AS product_name, p.stock_quantity, total_value AS stock_value
FROM hardware_vault.product p
JOIN (
    SELECT id, SUM(stock_quantity * price) AS total_value
    FROM hardware_vault.product
    GROUP BY id
) AS stock_value ON p.id = stock_value.id;

-- Refresh Command for Materialized View
REFRESH MATERIALIZED VIEW hardware_vault.product_stock_summary;

-- Materialized View: All product revenue
CREATE MATERIALIZED VIEW hardware_vault.all_product_revenue AS
SELECT p.id AS product_id, p.name AS product_name, SUM(po.price / 1.21 ) AS total_revenue
FROM hardware_vault.places_order po
JOIN hardware_vault.product p ON po.product_id = p.id
GROUP BY p.id, p.name
ORDER BY total_revenue DESC;

REFRESH MATERIALIZED VIEW hardware_vault.all_product_revenue;

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

-- Rule 4: Update customer order count (trigger when new order is placed)
CREATE OR REPLACE FUNCTION update_order_count() RETURNS TRIGGER AS $$
BEGIN
    UPDATE hardware_vault.customer
    SET order_count = (
        SELECT COUNT(*)
        FROM hardware_vault.places_order
        WHERE customer_id = NEW.customer_id
    )
    WHERE id = NEW.customer_id;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_update_order_count
AFTER INSERT OR UPDATE ON hardware_vault.places_order
FOR EACH ROW EXECUTE FUNCTION update_order_count();

-- Rule 5: Stock Replenishment Alert (trigger when stock falls below 10)
CREATE OR REPLACE FUNCTION stock_replenishment_alert() RETURNS TRIGGER AS $$
BEGIN
    IF NEW.stock_quantity < 10 THEN
        INSERT INTO hardware_vault.stock_alert (product_id, alert_message)
        VALUES (NEW.product_id, 'Stock is below 10, consider replenishing.');
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_stock_replenishment_alert
AFTER UPDATE ON hardware_vault.has_product
FOR EACH ROW EXECUTE FUNCTION stock_replenishment_alert();

-- DROP TRIGGER trg_stock_replenishment_alert ON hardware_vault.has_product;

-- Rule 6: Calculate order revenue based on price, quantity and delivery price
CREATE OR REPLACE FUNCTION calculate_order_price()
RETURNS TRIGGER AS $$
DECLARE
    product_price DECIMAL(10, 2);
    min_delivery_price DECIMAL(10, 2);
BEGIN
    -- Get the product's price from the product table
    SELECT price INTO product_price
    FROM hardware_vault.product
    WHERE id = NEW.product_id;

   -- Get the smallest delivery price from the supplier table for the product
    SELECT COALESCE(MIN(delivery_price), 0) INTO min_delivery_price
    FROM hardware_vault.supplier
    WHERE product_id = NEW.product_id;

    -- Calculate the price for the places_order table
    NEW.price := ((product_price * NEW.count) - min_delivery_price) * 1.21;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER set_order_price
BEFORE INSERT ON hardware_vault.places_order
FOR EACH ROW
EXECUTE FUNCTION calculate_order_price();

-- Rule 7: Stock quantity trigger
CREATE OR REPLACE FUNCTION handle_order_stock()
RETURNS TRIGGER AS $$
DECLARE
    total_stock INTEGER;
    warehouse RECORD;
BEGIN
    -- Calculate the total stock for the ordered product across all warehouses
    SELECT SUM(stock_quantity) INTO total_stock
    FROM hardware_vault.has_product
    WHERE product_id = NEW.product_id;

    -- Check if there is enough stock
    IF total_stock IS NULL OR total_stock < NEW.count THEN
        RAISE EXCEPTION 'Insufficient stock for product ID %', NEW.product_id;
    END IF;

    -- Deduct the stock from the warehouses in order
    FOR warehouse IN
        SELECT warehouse_id, stock_quantity
        FROM hardware_vault.has_product
        WHERE product_id = NEW.product_id
        ORDER BY stock_quantity DESC -- Deduct from warehouses with more stock first
    LOOP
        IF warehouse.stock_quantity >= NEW.count THEN
            -- Deduct stock from this warehouse
            UPDATE hardware_vault.has_product
            SET stock_quantity = stock_quantity - NEW.count
            WHERE warehouse_id = warehouse.warehouse_id
            AND product_id = NEW.product_id;
            EXIT; -- Exit once stock is fulfilled
        ELSE
            -- Use all stock from this warehouse and continue to the next
            UPDATE hardware_vault.has_product
            SET stock_quantity = 0
            WHERE warehouse_id = warehouse.warehouse_id
            AND product_id = NEW.product_id;
            NEW.count := NEW.count - warehouse.stock_quantity; -- Remaining stock to deduct
        END IF;
    END LOOP;

    -- Update the product's total stock quantity
    UPDATE hardware_vault.product
    SET stock_quantity = stock_quantity - NEW.count
    WHERE id = NEW.product_id;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_place_order
BEFORE INSERT ON hardware_vault.places_order
FOR EACH ROW
EXECUTE FUNCTION handle_order_stock();

-- Rule 8: Replenish stock trigger
CREATE OR REPLACE FUNCTION replenish_stock()
RETURNS TRIGGER AS $$
DECLARE
    warehouse_count INTEGER;
    base_allocation INTEGER;
    remaining_stock INTEGER;
    warehouse RECORD;
BEGIN
    -- For each warehouse if has_product does not exist, insert with stock_quantity 0
    FOR warehouse IN
        SELECT id
        FROM hardware_vault.warehouse
    LOOP
        INSERT INTO hardware_vault.has_product (warehouse_id, product_id, stock_quantity)
        SELECT warehouse.id, NEW.id, 0
        WHERE NOT EXISTS (
            SELECT 1
            FROM hardware_vault.has_product
            WHERE warehouse_id = warehouse.id
            AND product_id = NEW.id
        );
    END LOOP;

    -- Count the number of warehouses storing this product
    SELECT COUNT(*) INTO warehouse_count
    FROM hardware_vault.has_product
    WHERE product_id = NEW.id;

    -- Calculate how much stock each warehouse should get
    base_allocation := NEW.stock_quantity / warehouse_count;
    remaining_stock := NEW.stock_quantity % warehouse_count;

    -- Distribute stock across all warehouses
    FOR warehouse IN
        SELECT warehouse_id
        FROM hardware_vault.has_product
        WHERE product_id = NEW.id
    LOOP
        -- Allocate base stock to each warehouse
        INSERT INTO hardware_vault.has_product (warehouse_id, product_id, stock_quantity)
        VALUES (warehouse.warehouse_id, NEW.id, base_allocation)
        ON CONFLICT (warehouse_id, product_id) DO UPDATE
        SET stock_quantity = hardware_vault.has_product.stock_quantity + EXCLUDED.stock_quantity;

        -- Distribute remaining stock to the first few warehouses
        IF remaining_stock > 0 THEN
            UPDATE hardware_vault.has_product
            SET stock_quantity = stock_quantity + 1
            WHERE warehouse_id = warehouse.warehouse_id
            AND product_id = NEW.id;
            remaining_stock := remaining_stock - 1;
        END IF;
    END LOOP;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER after_replenish_stock
AFTER INSERT OR UPDATE ON hardware_vault.product
FOR EACH ROW
EXECUTE FUNCTION replenish_stock();

-- Rule 9: If no product, not allowed to insert specification
CREATE OR REPLACE FUNCTION check_product_exists()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM hardware_vault.product
        WHERE id = NEW.product_id
    ) THEN
        RAISE EXCEPTION 'Product with ID % does not exist', NEW.product_id;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_check_product_exists
BEFORE INSERT ON hardware_vault.specification
FOR EACH ROW
EXECUTE FUNCTION check_product_exists();

-- Rule 10: Don't allow employee if no warehouse
CREATE OR REPLACE FUNCTION check_warehouse_exists()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM hardware_vault.warehouse
        WHERE address = NEW.warehouse_id
    ) THEN
        RAISE EXCEPTION 'Warehouse with ID % does not exist', NEW.warehouse_id;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_check_warehouse_exists
BEFORE INSERT ON hardware_vault.employee
FOR EACH ROW
EXECUTE FUNCTION check_warehouse_exists();

-- Rule 11: set order_count 0 for new customer
CREATE OR REPLACE FUNCTION set_order_count_to_zero()
RETURNS TRIGGER AS $$
BEGIN
    NEW.order_count := 0;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_insert_customer
BEFORE INSERT ON hardware_vault.customer
FOR EACH ROW
EXECUTE FUNCTION set_order_count_to_zero();

-- Insert test data for Organization Table
INSERT INTO hardware_vault.organization DEFAULT VALUES;
-- Insert test data for Warehouse Table
INSERT INTO hardware_vault.warehouse (address, organization_id) VALUES
('123 Main St', 1),
('456 Elm St', 1),
('789 Oak St', 1);

-- Insert test data for Category Table
INSERT INTO hardware_vault.category (name, description) VALUES
('CPU', 'Central Processing Unit'),
('GPU', 'Graphics Processing Unit'),
('Motherboard', 'Main circuit board'),
('RAM', 'Random Access Memory'),
('Power Supply', 'Power Supply Unit'),
('HDD', 'Hard Disk Drive'),
('SSD', 'Solid State Drive'),
('Monitor', 'Computer Monitor'),
('Keyboard', 'Computer Keyboard'),
('Mouse', 'Computer Mouse'),
('Headphones', 'Headphones'),
('Speakers', 'Computer Speakers'),
('Case', 'Computer Case'),
('Cooling', 'Cooling System'),
('Ethernet Card', 'Network Interface Card');


-- Insert test data for Product Table
INSERT INTO hardware_vault.product (category_id, name, description, price, stock_quantity) VALUES
(1, 'AMD Ryzen 9 7950X', '16 cores / 32 threads / 5.7 GHz boost. Zen 4 architecture.', 465.00, 15),
(1, 'Intel Core i5-14600K', '14 cores (6P + 8E) / 5.3 GHz boost. Excellent gaming and productivity.', 319.00, 30),
(1, 'AMD Ryzen 7 7700X', '8 cores / 16 threads / 4.5 GHz base clock. Great for AM5 gaming.', 259.00, 25),
(1, 'Intel Core i3-13100', '4 cores / 4.5 GHz boost. Entry-level CPU for basic tasks.', 99.00, 40),
(1, 'AMD Ryzen 5 5600X', '6 cores / 12 threads / 3.7 GHz base clock. Affordable gaming CPU.', 105.00, 35),
(1, 'AMD Ryzen 7 5800X3D', '8 cores / 16 threads / 3D V-Cache. Outstanding AM4 gaming performance.', 268.00, 10),
(1, 'Intel Core i5-13400F', '10 cores / 4.6 GHz boost. Lacks integrated graphics. Excellent value.', 162.00, 50),
(2, 'Nvidia GeForce RTX 4080', '16 GB VRAM, excellent 4K performance, ideal for gaming and AI tasks.', 1039.99, 30),
(2, 'AMD Radeon RX 7900 XT', '20 GB VRAM, superior rasterization performance, competitive with Nvidia.', 699.99, 40),
(2, 'Nvidia GeForce RTX 4070 Ti', '12 GB VRAM, solid ray tracing performance and high efficiency.', 652.53, 50),
(2, 'AMD Radeon RX 7800 XT', '16 GB VRAM, great value for 1440p gaming and content creation.', 489.99, 45),
(2, 'Nvidia GeForce RTX 4060 Ti', '16 GB VRAM, strong midrange performance with DLSS 3 support.', 449.99, 60),
(2, 'AMD Radeon RX 6700 XT', '12 GB VRAM, affordable option for 1080p to 1440p gaming.', 314.03, 70),
(2, 'Nvidia GeForce RTX 3060 Ti', '8 GB VRAM, budget-friendly GPU with reliable performance.', 325.47, 65),
(2, 'AMD Radeon RX 6650 XT', '8 GB VRAM, great efficiency and cost-effectiveness for 1080p gaming.', 220.54, 80),
(2, 'Intel Arc A750', '8 GB VRAM, entry-level GPU with surprising performance for the price.', 209.99, 50),
(2, 'AMD Radeon RX 580', '8 GB VRAM, older model but still capable for 1080p gaming.', 99.98, 90),
(3, 'Gigabyte Z790 Aorus Xtreme', 'High-performance motherboard with Intel Z790 chipset, supports PCIe 5.0 and DDR5 RAM, WiFi 6E.', 799.99, 30),
(3, 'MSI MAG B760M Mortar WiFi', 'Micro ATX board with Intel B760 chipset, PCIe 4.0, DDR5 support, and built-in WiFi 6.', 179.95, 50),
(3, 'ASRock Z790 Taichi Lite', 'Premium Intel Z790 motherboard with DDR5 RAM support, PCIe 5.0, and advanced cooling features.', 464.44, 20),
(3, 'MSI MPG B650I Edge ITX', 'Compact AM5 motherboard with DDR5 support, PCIe 4.0, WiFi 6E, and Bluetooth.', 269.99, 40),
(3, 'ASRock B550M-ITX/ac', 'Value-oriented AM4 motherboard for small builds, supports DDR4 RAM and PCIe 4.0.', 121.99, 60),
(4, 'Corsair Vengeance DDR5 32GB', 'High-speed DDR5 RAM with 5600MHz frequency, ideal for gaming and productivity.', 199.99, 100),
(4, 'G.SKILL Trident Z5 RGB 16GB', 'Premium DDR5 RAM with RGB lighting, 6000MHz speed, and low latency.', 149.99, 75),
(4, 'Kingston Fury Beast 32GB DDR4', 'Affordable DDR4 RAM with 3600MHz speed, great for mainstream systems.', 99.99, 150),
(4, 'TeamGroup T-Force Delta RGB DDR5 16GB', 'Stylish DDR5 RAM with RGB lighting and 6400MHz clock speed.', 159.99, 80),
(4, 'Crucial Ballistix 64GB DDR4', 'High-capacity DDR4 memory with 3200MHz speed for heavy multitasking.', 249.99, 50),
(5, 'NZXT C1500 Platinum', '1500W, 80 Plus Platinum, modular design, supports dual GPUs', 370.00, 20),
(5, 'Seasonic Prime TX-1600', '1600W, Titanium efficiency, modular, premium build for extreme systems', 600.00, 15),
(5, 'Corsair SF750 SFX', '750W, compact SFX form factor, 80 Plus Platinum, modular', 175.00, 25),
(6, 'WD Black 8TB', 'High-performance internal HDD for gaming and storage, 7200 RPM', 174.99, 40),
(6, 'Seagate IronWolf Pro 20TB', 'NAS-optimized, 7200 RPM, designed for 24/7 usage', 299.99, 10),
(6, 'WD Gold 22TB', 'Enterprise-grade HDD, 7200 RPM, designed for reliability', 449.99, 5),
(7, 'Samsung 990 Pro 2TB', 'High-performance PCIe 4.0 NVMe SSD, up to 7450 MB/s read speed', 159.99, 50),
(7, 'Crucial T700 2TB', 'PCIe 5.0 NVMe SSD, 12,400 MB/s read speed, heatsink included', 299.99, 30),
(7, 'WD Black SN850X 4TB', 'PCIe 4.0 SSD, up to 7300 MB/s read speed, gaming-optimized', 349.99, 20),
(8, 'Dell G2724D', '27-inch, 1440p, 165Hz refresh rate, IPS panel.', 199.99, 45),
(8, 'Dell U2723QE', '27-inch, 4K resolution, wide color gamut, KVM switch.', 399.99, 30),
(8, 'ASUS ProArt Display PA279CRV', '27-inch, 4K resolution, high color accuracy for editing.', 549.99, 25),
(9, 'SteelSeries Apex Pro TKL', 'Compact mechanical gaming keyboard with OLED display.', 189.99, 50),
(9, 'Logitech MX Keys', 'Slim wireless keyboard with backlighting and multi-device support.', 119.99, 60),
(9, 'Corsair K70 RGB Pro', 'Full-size mechanical keyboard with per-key RGB lighting.', 169.99, 40),
(10, 'Razer DeathAdder V3 Pro', 'Wireless gaming mouse, 30,000 DPI, lightweight design.', 149.99, 35),
(10, 'Corsair Katar Pro XT', 'Wired gaming mouse, 18,000 DPI, lightweight and ergonomic.', 39.99, 50),
(10, 'Logitech MX Master 3S', 'Advanced wireless mouse with precision scrolling and ergonomic design.', 99.99, 45),
(11, 'Sony WH-1000XM5', 'Noise-cancelling over-ear headphones with premium sound and long battery life.', 349.99, 40),
(11, 'Bose QuietComfort 45', 'Wireless over-ear headphones with world-class noise cancellation and comfort.', 329.99, 30),
(11, 'Sennheiser Momentum 4', 'High-quality wireless headphones with deep bass and advanced noise cancellation.', 379.99, 25),
(12, 'Logitech Z623', '2.1 speaker system with THX certification for rich bass and clear audio.', 149.99, 50),
(12, 'Creative Pebble 2.0', 'Compact desktop speakers with clear sound for small setups.', 19.99, 100),
(12, 'Razer Nommo Chroma', 'RGB 2.0 gaming speakers with powerful sound and adjustable bass.', 149.99, 45),
(13, 'NZXT H510', 'Mid-tower case with sleek design, tempered glass, and good airflow.', 69.99, 60),
(13, 'Corsair iCUE 4000X RGB', 'Mid-tower case with RGB fans, tempered glass panels, and good cable management.', 109.99, 50),
(13, 'Fractal Design Meshify C', 'Compact mid-tower case with mesh front panel for superior airflow.', 89.99, 45),
(14, 'Corsair iCUE H100i ELITE', '240mm all-in-one liquid cooler with RGB fans for CPU cooling.', 159.99, 30),
(14, 'Noctua NH-D15', 'Premium air cooler with dual tower and dual fan design for superior heat dissipation.', 89.99, 40),
(14, 'Cooler Master Hyper 212 Black Edition', 'Air cooler with 4 heat pipes and a 120mm fan for efficient cooling.', 39.99, 50),
(15, 'TP-Link Archer TX3000E', 'WiFi 6 PCIe card with Bluetooth 5.0 for high-speed networking.', 79.99, 30),
(15, 'Intel I210-T1', 'Gigabit Ethernet network card with low power consumption and high reliability.', 49.99, 50),
(15, 'Asus XG-C100C', '10Gbps Ethernet card for high-speed networking with low latency.', 179.99, 25),
(15, 'Dell X540-T2', 'Dual-port 10Gbps Ethernet card for server and workstation connectivity.', 249.99, 20);

-- Insert test data for Supplier Table
INSERT INTO hardware_vault.supplier (phone_number, email, name, rating, address, product_id, delivery_price) VALUES
('8686865568','supplier1@example.com', 'Supplier One', 4.5, '123 Supplier St', 127, 2.78),
('8686868686','supplier2@example.com', 'Supplier Two', 3.8, '456 Supplier St', 130, 5.15),
('6581898455','supplier3@example.com', 'Supplier Three', 4.5, '1233 Supplier St', 131, 3.17),
('6546588155','supplier4@example.com', 'Supplier Four', 3.8, '4526 Supplier St', 132, 1.99);

INSERT INTO hardware_vault.specification (name, description, value, product_id) VALUES
('Cores', 'Number of processor cores', '16 cores (8P + 8E)', 1),
('Base Clock', 'Base clock speed', '3.2 GHz', 1),
('Boost Clock', 'Boost clock speed', '5.2 GHz', 1),
('Architecture', 'CPU architecture', 'Alder Lake', 1),
('Socket', 'Processor socket type', 'LGA 1700', 1),
('Cores', 'Number of processor cores', '16 cores', 2),
('Threads', 'Number of processor threads', '32 threads', 2),
('Base Clock', 'Base clock speed', '3.4 GHz', 2),
('Boost Clock', 'Boost clock speed', '5.7 GHz', 2),
('Architecture', 'CPU architecture', 'Zen 4', 2),
('Socket', 'Processor socket type', 'AM5', 2),
('Cores', 'Number of processor cores', '14 cores (6P + 8E)', 3),
('Base Clock', 'Base clock speed', '3.0 GHz', 3),
('Boost Clock', 'Boost clock speed', '5.3 GHz', 3),
('Architecture', 'CPU architecture', 'Alder Lake', 3),
('Socket', 'Processor socket type', 'LGA 1700', 3),
('Cores', 'Number of processor cores', '8 cores', 4),
('Threads', 'Number of processor threads', '16 threads', 4),
('Base Clock', 'Base clock speed', '4.5 GHz', 4),
('Boost Clock', 'Boost clock speed', '5.4 GHz', 4),
('Architecture', 'CPU architecture', 'Zen 4', 4),
('Socket', 'Processor socket type', 'AM5', 4),
('Cores', 'Number of processor cores', '4 cores', 5),
('Base Clock', 'Base clock speed', '3.5 GHz', 5),
('Boost Clock', 'Boost clock speed', '4.5 GHz', 5),
('Architecture', 'CPU architecture', 'Alder Lake', 5),
('Socket', 'Processor socket type', 'LGA 1700', 5),
('Cores', 'Number of processor cores', '6 cores', 6),
('Threads', 'Number of processor threads', '12 threads', 6),
('Base Clock', 'Base clock speed', '3.6 GHz', 6),
('Boost Clock', 'Boost clock speed', '4.6 GHz', 6),
('Architecture', 'CPU architecture', 'Zen 3', 6),
('Socket', 'Processor socket type', 'AM4', 6),
('Cores', 'Number of processor cores', '8 cores', 7),
('Threads', 'Number of processor threads', '16 threads', 7),
('Base Clock', 'Base clock speed', '3.4 GHz', 7),
('Boost Clock', 'Boost clock speed', '4.6 GHz', 7),
('Architecture', 'CPU architecture', 'Zen 3D', 7),
('Socket', 'Processor socket type', 'AM4', 7),
('Cores', 'Number of processor cores', '10 cores', 8),
('Base Clock', 'Base clock speed', '3.2 GHz', 8),
('Boost Clock', 'Boost clock speed', '4.6 GHz', 8),
('Architecture', 'CPU architecture', 'Alder Lake', 8),
('Socket', 'Processor socket type', 'LGA 1700', 8),
('VRAM', 'Video memory size', '16 GB', 9),
('Architecture', 'GPU architecture', 'Ampere', 9),
('Boost Clock', 'Boost clock speed', '2.3 GHz', 9),
('Memory Type', 'Memory technology', 'GDDR6X', 9),
('VRAM', 'Video memory size', '20 GB', 10),
('Architecture', 'GPU architecture', 'RDNA 3', 10),
('Boost Clock', 'Boost clock speed', '2.5 GHz', 10),
('Memory Type', 'Memory technology', 'GDDR6', 10),
('VRAM', 'Video memory size', '12 GB', 11),
('Architecture', 'GPU architecture', 'Ampere', 11),
('Boost Clock', 'Boost clock speed', '2.2 GHz', 11),
('Memory Type', 'Memory technology', 'GDDR6X', 11),
('VRAM', 'Video memory size', '16 GB', 12),
('Architecture', 'GPU architecture', 'RDNA 3', 12),
('Boost Clock', 'Boost clock speed', '2.3 GHz', 12),
('Memory Type', 'Memory technology', 'GDDR6', 12),
('VRAM', 'Video memory size', '16 GB', 13),
('Architecture', 'GPU architecture', 'Ampere', 13),
('Boost Clock', 'Boost clock speed', '2.1 GHz', 13),
('Memory Type', 'Memory technology', 'GDDR6X', 13),
('VRAM', 'Video memory size', '12 GB', 14),
('Architecture', 'GPU architecture', 'RDNA 3', 14),
('Boost Clock', 'Boost clock speed', '2.0 GHz', 14),
('Memory Type', 'Memory technology', 'GDDR6', 14),
('VRAM', 'Video memory size', '8 GB', 15),
('Architecture', 'GPU architecture', 'Ampere', 15),
('Boost Clock', 'Boost clock speed', '1.7 GHz', 15),
('Memory Type', 'Memory technology', 'GDDR6X', 15),
('VRAM', 'Video memory size', '8 GB', 16),
('Architecture', 'GPU architecture', 'RDNA 3', 16),
('Boost Clock', 'Boost clock speed', '1.8 GHz', 16),
('Memory Type', 'Memory technology', 'GDDR6', 16),
('VRAM', 'Video memory size', '8 GB', 17),
('Architecture', 'GPU architecture', 'Alchemist', 17),
('Boost Clock', 'Boost clock speed', '1.9 GHz', 17),
('Memory Type', 'Memory technology', 'GDDR6', 17),
('VRAM', 'Video memory size', '8 GB', 18),
('Architecture', 'GPU architecture', 'Polaris', 18),
('Boost Clock', 'Boost clock speed', '1.3 GHz', 18),
('Memory Type', 'Memory technology', 'GDDR5', 18),
('Chipset', 'Motherboard chipset', 'Intel Z790', 19),
('Form Factor', 'Motherboard size', 'ATX', 19),
('Memory Type', 'Memory technology', 'DDR5', 19),
('Expansion Slots', 'PCIe slots', '3 x PCIe 5.0', 19),
('Chipset', 'Motherboard chipset', 'Intel B760', 20),
('Form Factor', 'Motherboard size', 'Micro ATX', 20),
('Memory Type', 'Memory technology', 'DDR5', 20),
('Expansion Slots', 'PCIe slots', '2 x PCIe 4.0', 20),
('Chipset', 'Motherboard chipset', 'Intel Z790', 21),
('Form Factor', 'Motherboard size', 'ATX', 21),
('Memory Type', 'Memory technology', 'DDR5', 21),
('Expansion Slots', 'PCIe slots', '3 x PCIe 5.0', 21),
('Chipset', 'Motherboard chipset', 'AMD AM5', 22),
('Form Factor', 'Motherboard size', 'Mini ITX', 22),
('Memory Type', 'Memory technology', 'DDR5', 22),
('Expansion Slots', 'PCIe slots', '1 x PCIe 4.0', 22),  
('Capacity', 'Memory capacity', '32 GB', 23),
('Speed', 'Memory speed', '5600 MHz', 23),
('Type', 'Memory type', 'DDR5', 23),
('Latency', 'Memory latency', 'CL36', 23),
('Capacity', 'Memory capacity', '16 GB', 24),
('Speed', 'Memory speed', '6000 MHz', 24),
('Type', 'Memory type', 'DDR5', 24),
('Latency', 'Memory latency', 'CL32', 24),
('Capacity', 'Memory capacity', '32 GB', 25),
('Speed', 'Memory speed', '3600 MHz', 25),
('Type', 'Memory type', 'DDR4', 25),
('Latency', 'Memory latency', 'CL18', 25),
('Capacity', 'Memory capacity', '16 GB', 26),
('Speed', 'Memory speed', '6400 MHz', 26),
('Type', 'Memory type', 'DDR5', 26),
('Latency', 'Memory latency', 'CL34', 26),
('Capacity', 'Memory capacity', '64 GB', 27),
('Speed', 'Memory speed', '3200 MHz', 27),
('Type', 'Memory type', 'DDR4', 27),
('Latency', 'Memory latency', 'CL16', 27),
('Wattage', 'Power supply wattage', '1500W', 28),
('Efficiency', 'Power supply efficiency', '80 Plus Platinum', 28),
('Modularity', 'Power supply modularity', 'Fully modular', 28),
('Features', 'Power supply features', 'Dual GPU support', 28),
('Wattage', 'Power supply wattage', '1600W', 29),
('Efficiency', 'Power supply efficiency', '80 Plus Titanium', 29),
('Modularity', 'Power supply modularity', 'Fully modular', 29),
('Features', 'Power supply features', 'Extreme system support', 29),
('Wattage', 'Power supply wattage', '750W', 30),
('Efficiency', 'Power supply efficiency', '80 Plus Platinum', 30),
('Modularity', 'Power supply modularity', 'Fully modular', 30),
('Features', 'Power supply features', 'Compact SFX design', 30),
('Capacity', 'Hard drive capacity', '8TB', 31),
('Speed', 'Hard drive speed', '7200 RPM', 31),
('Form Factor', 'Hard drive form factor', '3.5"', 31),
('Usage', 'Hard drive usage', 'Gaming and storage', 31),
('Capacity', 'Hard drive capacity', '20TB', 32),
('Speed', 'Hard drive speed', '7200 RPM', 32),
('Form Factor', 'Hard drive form factor', '3.5"', 32),
('Usage', 'Hard drive usage', 'NAS-optimized', 32),
('Capacity', 'Hard drive capacity', '22TB', 33),
('Speed', 'Hard drive speed', '7200 RPM', 33),
('Form Factor', 'Hard drive form factor', '3.5"', 33),
('Usage', 'Hard drive usage', 'Enterprise-grade', 33),
('Capacity', 'SSD capacity', '2TB', 34),
('Speed', 'SSD speed', '7450 MB/s', 34),
('Interface', 'SSD interface', 'PCIe 4.0', 34),
('Form Factor', 'SSD form factor', 'M.2 2280', 34),
('Capacity', 'SSD capacity', '2TB', 35),
('Speed', 'SSD speed', '12400 MB/s', 35),
('Interface', 'SSD interface', 'PCIe 5.0', 35),
('Form Factor', 'SSD form factor', 'M.2 2280', 35),
('Capacity', 'SSD capacity', '4TB', 36),
('Speed', 'SSD speed', '7300 MB/s', 36),
('Interface', 'SSD interface', 'PCIe 4.0', 36),
('Form Factor', 'SSD form factor', 'M.2 2280', 36),
('Size', 'Monitor size', '27 inches', 37),
('Resolution', 'Monitor resolution', '1440p', 37),
('Refresh Rate', 'Monitor refresh rate', '165Hz', 37),
('Panel Type', 'Monitor panel type', 'IPS', 37),
('Size', 'Monitor size', '27 inches', 38),
('Resolution', 'Monitor resolution', '4K', 38),
('Refresh Rate', 'Monitor refresh rate', '60Hz', 38),
('Panel Type', 'Monitor panel type', 'IPS', 38),
('Size', 'Monitor size', '27 inches', 39),
('Resolution', 'Monitor resolution', '4K', 39),
('Refresh Rate', 'Monitor refresh rate', '60Hz', 39),
('Panel Type', 'Monitor panel type', 'IPS', 39),
('Switch Type', 'Keyboard switch type', 'Omron', 40),
('Layout', 'Keyboard layout', 'Tenkeyless', 40),
('Features', 'Keyboard features', 'OLED display', 40),
('Switch Type', 'Keyboard switch type', 'Membrane', 41),
('Layout', 'Keyboard layout', 'Full-size', 41),
('Features', 'Keyboard features', 'Backlighting', 41),
('Switch Type', 'Keyboard switch type', 'Cherry MX', 42),
('Layout', 'Keyboard layout', 'Full-size', 42),
('Features', 'Keyboard features', 'Per-key RGB', 42),
('Sensor Type', 'Mouse sensor type', 'Optical', 43),
('DPI', 'Mouse DPI', '30,000', 43),
('Connectivity', 'Mouse connectivity', 'Wireless', 43),
('Weight', 'Mouse weight', '74g', 43),
('Sensor Type', 'Mouse sensor type', 'Optical', 44),
('DPI', 'Mouse DPI', '18,000', 44),
('Connectivity', 'Mouse connectivity', 'Wired', 44),
('Weight', 'Mouse weight', '69g', 44),
('Sensor Type', 'Mouse sensor type', 'Laser', 45),
('DPI', 'Mouse DPI', '4,000', 45),
('Connectivity', 'Mouse connectivity', 'Wireless', 45),
('Weight', 'Mouse weight', '141g', 45),
('Type', 'Headphone type', 'Over-ear', 46),
('Noise Cancellation', 'Headphone noise cancellation', 'Active', 46),
('Battery Life', 'Headphone battery life', '30 hours', 46),
('Sound Quality', 'Headphone sound quality', 'High-resolution', 46),
('Type', 'Headphone type', 'Over-ear', 47),
('Noise Cancellation', 'Headphone noise cancellation', 'Active', 47),
('Battery Life', 'Headphone battery life', '24 hours', 47),
('Sound Quality', 'Headphone sound quality', 'High-resolution', 47),
('Type', 'Headphone type', 'Over-ear', 48),
('Noise Cancellation', 'Headphone noise cancellation', 'Active', 48),
('Battery Life', 'Headphone battery life', '28 hours', 48),
('Sound Quality', 'Headphone sound quality', 'High-resolution', 48),
('Type', 'Speaker type', '2.1', 49),
('Certification', 'Speaker certification', 'THX', 49),
('Features', 'Speaker features', 'Rich bass', 49),
('Type', 'Speaker type', '2.0', 50),
('Features', 'Speaker features', 'Compact', 50),
('Sound Quality', 'Speaker sound quality', 'Clear', 50),
('Type', 'Speaker type', '2.0', 51),
('Features', 'Speaker features', 'RGB lighting', 51),
('Sound Quality', 'Speaker sound quality', 'Powerful', 51),
('Size', 'Case size', 'Mid-tower', 52),
('Design', 'Case design', 'Sleek', 52),
('Features', 'Case features', 'Tempered glass', 52),
('Size', 'Case size', 'Mid-tower', 53),
('Design', 'Case design', 'RGB fans', 53),
('Features', 'Case features', 'Cable management', 53),
('Size', 'Case size', 'Mid-tower', 54),
('Design', 'Case design', 'Mesh front panel', 54),
('Features', 'Case features', 'Airflow', 54),
('Size', 'Cooler size', '240mm', 55),
('Type', 'Cooler type', 'Liquid', 55),
('Features', 'Cooler features', 'RGB fans', 55),
('Size', 'Cooler size', 'Dual tower', 56),
('Type', 'Cooler type', 'Air', 56),
('Features', 'Cooler features', 'Dual fan', 56),
('Size', 'Cooler size', '120mm', 57),
('Type', 'Cooler type', 'Air', 57),
('Features', 'Cooler features', '4 heat pipes', 57),
('Type', 'Network card type', 'WiFi 6', 58),
('Speed', 'Network card speed', '10Gbps', 58),
('Features', 'Network card features', 'Bluetooth 5.0', 58),
('Type', 'Network card type', 'Gigabit Ethernet', 59),
('Power', 'Network card power', 'Low', 59),
('Reliability', 'Network card reliability', 'High', 59),
('Type', 'Network card type', '10Gbps Ethernet', 60),
('Speed', 'Network card speed', '10Gbps', 60),
('Latency', 'Network card latency', 'Low', 60);

-- Insert test data for Employee Table
INSERT INTO hardware_vault.employee (position, years_of_experience, contact_info, warehouse_id, first_name, last_name) VALUES
('Manager', 5, 'manager@example.com', '123 Main St', 'John', 'Doe'),
('Worker', 2, 'worker@example.com', '456 Elm St', 'Jane', 'Smith'),
('Worker', 2, 'worker2@example.com', '789 Oak St', 'Alice', 'Johnson'),
('Manager', 3, 'manager2@example.com', '789 Oak St', 'Bob', 'Williams');

-- ('123 Main St', 1),
-- ('456 Elm St', 1),
-- ('789 Oak St', 1);
-- Insert test data for Customer Table
INSERT INTO hardware_vault.customer (contact_info, first_name, last_name) VALUES
('customer1@example.com', 'Alice', 'Johnson'),
('customer2@example.com', 'Bob', 'Williams'),
('customer3@example.com', 'Charlie', 'Brown'),
('customer4@example.com', 'David', 'Clark'),
('customer5@example.com', 'Eve', 'Davis'),
('customer6@example.com', 'Frank', 'Miller'),
('customer7@example.com', 'Grace', 'Lee');

-- Insert test data for Places Order Table
INSERT INTO hardware_vault.places_order (customer_id, product_id, count) VALUES
(3, 3, 3),
(4, 4, 4),
(2, 10, 5),
(1, 1, 1),
(1, 3, 2),
(2, 4, 5),
(3, 5, 3),
(4, 6, 1),
(5, 7, 2),
(6, 8, 1),
(7, 9, 3),
(2, 10, 5);

-- Insert test data for Price Change Log Table
INSERT INTO hardware_vault.price_change_log (product_id, old_price, new_price) VALUES
(1, 299.99, 589.99),
(2, 199.99, 465.00),
(16, 79.99, 220.54);


-- Insert test data for Stock Alert Table
INSERT INTO hardware_vault.stock_alert (product_id, alert_message) VALUES
(1, 'Stock is below 10, consider replenishing.'),
(7, 'Stock is below 10, consider replenishing.'),
(32, 'Stock is below 10, consider replenishing.');

