# Joins

## cross join

- performs cartesian product of two sets/tables
- returns all possible combinations b/w the 2 tables

```sql
select * from A cross join B
```

### exercises

1.  cross join without clause

```sql
select * from 
departments d ,locations l 
```

- check via `select count(*) from  departments d ,locations l` vs  `select count(*) from departments d ,locations l where d.location_id=l.location_id;`

## inner join

- only returns the intersecting region

```sql
select e.first_name,d.department_name from 
employees e 
inner join 
departments d 
on d.department_id=e.department_id;
```

- inner join without clause

```sql
select * from 
departments d ,locations l 
where d.location_id=l.location_id;
```

### exercises

1. show each employee and their manager, (both are in the same table)

```sql

select e.employee_id, e.first_name, e2.employee_id, e2.first_name as manager 
from employees e inner join employees e2 
on e.employee_id = e2.manager_id
```

2. show employee name, their department and their location

```sql
select e.first_name, d.department_name, l.city 
from employees e 
inner join departments d on e.department_id = d.department_id 
inner join locations l on d.location_id = l.location_id;
```
or
```sql
select e.first_name, d.department_name, l.city from employees e, departments d, locations l
where e.department_id = d.department_id and d.location_id = l.location_id;
```

## union

- combines result of 2 queries and removes duplications

## intersect

- returns what is common


# views

- add alias to a query
- essentially saves the query as a table with name being the alias


```sql
create view emp_basic as
    select * from employees;
    
select * from emp_basic
```

## exercises

- use views to get all employees from oxford

```sql
-- prviously defined query that joins employee with their city
create view emp_oxford as
    select e.first_name, d.department_name, l.city 
    from employees e 
    inner join departments d on e.department_id = d.department_id 
    inner join locations l on d.location_id = l.location_id ;    
    
-- filtering from that query, using view to simplify our custom prompt
select * from emp_oxford where city = 'Oxford'
```