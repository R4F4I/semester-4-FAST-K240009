select * from courses;
select * from departments;
select * from employees;
select * from enrollments;
select * from payments;
select * from students;

select sum(salary), avg(salary), avg(nvl(salary,0))
from employees


select emp_name, NVL2(salary,'fixed','hourly') as salary_status, 
from employees


select 
    round(avg(e.salary),2)as avg_sal,
    min(e.salary) as min_sal,
    d.dept_name 
from employees e inner join departments d
on d.dept_id = e.dept_id
group by d.dept_name
having avg(e.salary) > 50000
order by min(e.salary) asc


SELECT 
    dept_id, 
    MIN(salary) AS min_sal, 
    AVG(salary) AS avg_sal
FROM employees
GROUP BY dept_id
HAVING AVG(salary) > 50000
ORDER BY min_sal ASC;


select * from employees



alter table employees
add (bonus number)


update employees
set bonus = (salary * 1.05)

select 
    emp_name, 
    bonus ,
    case 
        when bonus >= 50000 then 'Top Tier'
        when bonus >= 10000 then 'standard Tier'
        else 'no bonus'
    end as bonus_bracket
from employees


select e.emp_name, d.dept_name 
from employees e inner join departments d
on e.dept_id = d.dept_id

select e.emp_name, d.dept_name 
from departments d left join employees e 
on e.dept_id = d.dept_id

select * from departments 
order by dept_id

insert into departments values (6, 'chemistry')


create table projects(

    project_id number primary key,
    project_name varchar(100) not null,
    budget number(10,2) check (budget >= 0)
)

create table assignments(

    assign_id       number          primary key,
    emp_id          number,          
    project_id      number,          
    
    hours_logged    number          default 0,
    budget          number(10,2)    check (budget >= 0),
    
    constraint emp_id       foreign key (emp_id)       references employees(emp_id), 
    constraint project_id   foreign key (project_id)   references projects(project_id)
)



create table assignments(

    assign_id       number          primary key,
    emp_id          number          references employees(emp_id),           
    project_id      number          references projects(project_id),
    
    hours_logged    number          default 0,
    budget          number(10,2)    check (budget >= 0)

)


create table salary_audit(

    audit_id number primary key,
    emp_id number references employees(emp_id),
    old_salary number(10,2),
    new_salary number(10,2),
    change_date date
)

create index idx_emp_salary on employees(salary)


