import random

def generate_advanced_sql():
    with open("advanced_hr_data.sql", "w") as f:
        f.write("-- ==========================================\n")
        f.write("-- DML: POPULATING ADVANCED HR SCHEMA\n")
        f.write("-- ==========================================\n\n")

        # 1. Insert Departments
        departments = ['Executive', 'IT', 'Finance', 'HR', 'Operations']
        f.write("-- DEPARTMENTS\n")
        for i, dept in enumerate(departments, 1):
            f.write(f"INSERT INTO departments (dept_id, dept_name) VALUES ({i}, '{dept}');\n")
        f.write("\n")

        # 2. Insert Managers (emp_id 1 to 5)
        names = ["Ali", "Sara", "John", "Zahra", "Michael", "Emma", "Bilal", "Aisha", "Omar", "Fatima"]
        last_names = ["Khan", "Smith", "Ahmed", "Ali", "Shah", "Malik", "Jones", "Iqbal"]
        
        f.write("-- MANAGERS (Top Level)\n")
        for i in range(1, 6):
            name = f"{random.choice(names)} {random.choice(last_names)}"
            email = f"manager{i}@company.com"
            f.write(f"INSERT INTO employees (emp_id, emp_name, email, salary, manager_id, dept_id) "
                    f"VALUES ({i}, '{name}', '{email}', {random.randint(90000, 150000)}, NULL, {i});\n")
        f.write("\n")

        # 3. Insert Regular Employees (emp_id 6 to 50)
        f.write("-- REGULAR EMPLOYEES\n")
        for i in range(6, 51):
            name = f"{random.choice(names)} {random.choice(last_names)}"
            email = f"emp{i}@company.com"
            salary = random.randint(40000, 85000)
            manager_id = random.randint(1, 5) # Assign to one of the managers
            dept_id = manager_id # Keep them in their manager's department
            f.write(f"INSERT INTO employees (emp_id, emp_name, email, salary, manager_id, dept_id) "
                    f"VALUES ({i}, '{name}', '{email}', {salary}, {manager_id}, {dept_id});\n")
        f.write("\n")

        # 4. Insert Projects
        f.write("-- PROJECTS\n")
        projects = ['Cloud Migration', 'Q3 Audit', 'New App Launch', 'Office Move', 'Security Patch']
        for i, proj in enumerate(projects, 1):
            budget = random.randint(10000, 100000)
            f.write(f"INSERT INTO projects (project_id, project_name, budget) VALUES ({i}, '{proj}', {budget});\n")
        f.write("\n")

        # 5. Insert Assignments (Assigning employees to projects)
        f.write("-- ASSIGNMENTS\n")
        for i in range(1, 101):
            emp_id = random.randint(1, 50)
            proj_id = random.randint(1, 5)
            hours = random.randint(5, 120)
            
            f.write(f"INSERT INTO assignments (assign_id, emp_id, project_id, hours_logged) "
                    f"VALUES ({i}, {emp_id}, {proj_id}, {hours});\n")
        
        f.write("\nCOMMIT;\n")
        print("File 'advanced_hr_data.sql' generated successfully!")

if __name__ == "__main__":
    generate_advanced_sql()